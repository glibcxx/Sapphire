#include "DX12Hook.h"

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.Foundation.h>

#include <kiero/kiero.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/input/InputManager.h"

#include "Core.h"

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;

bool DX12Hook::CreateRenderTargetResources(IDXGISwapChain *pSwapChain) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    const UINT bufferCount = desc.BufferCount > APP_NUM_BACK_BUFFERS ? APP_NUM_BACK_BUFFERS : desc.BufferCount;

    for (UINT i = 0; i < bufferCount; ++i) {
        if (FAILED(pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pd3dMainRenderTargetResource[i]))))
            return false;

        D3D11_RESOURCE_FLAGS d3d11Flags = {D3D11_BIND_RENDER_TARGET};
        if (FAILED(pd3d11On12Device->CreateWrappedResource(
                pd3dMainRenderTargetResource[i],
                &d3d11Flags,
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                D3D12_RESOURCE_STATE_PRESENT,
                IID_PPV_ARGS(&pd3d11WrappedBackBuffer[i])
            ))) return false;

        if (FAILED(pd3d11Device->CreateRenderTargetView(pd3d11WrappedBackBuffer[i], nullptr, &pd3d11RenderTargetView[i])))
            return false;
    }
    return true;
}

void DX12Hook::CleanupRenderTargetResources() {
    for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; ++i) {
        if (pd3d11RenderTargetView[i]) {
            pd3d11RenderTargetView[i]->Release();
            pd3d11RenderTargetView[i] = nullptr;
        }
        if (pd3d11WrappedBackBuffer[i]) {
            pd3d11WrappedBackBuffer[i]->Release();
            pd3d11WrappedBackBuffer[i] = nullptr;
        }
        if (pd3dMainRenderTargetResource[i]) {
            pd3dMainRenderTargetResource[i]->Release();
            pd3dMainRenderTargetResource[i] = nullptr;
        }
    }
}

void DX12Hook::WaitForGPU() {
    if (!pd3dDevice && !pd3dCommandQueue)
        return;
    ID3D12Fence *fence;
    if (SUCCEEDED(pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
        const UINT64 fenceValue = 1;
        pd3dCommandQueue->Signal(fence, fenceValue);
        if (fence->GetCompletedValue() < fenceValue) {
            HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (event) {
                fence->SetEventOnCompletion(fenceValue, event);
                WaitForSingleObject(event, INFINITE);
                CloseHandle(event);
            }
        }
        fence->Release();
    }
}

HWND DX12Hook::FindChildWindowByClass(HWND parent, const wchar_t *className) {
    HWND hChild = nullptr;
    while ((hChild = FindWindowExW(parent, hChild, nullptr, nullptr))) {
        wchar_t buf[256];
        GetClassName(hChild, buf, 256);
        if (wcscmp(buf, className) == 0)
            return hChild;
        // 递归搜索子窗口的子窗口
        HWND result = FindChildWindowByClass(hChild, className);
        if (result) return result;
    }
    return nullptr;
}

void __stdcall DX12Hook::hkExecuteCommandLists(ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists) {
    if (!pd3dCommandQueue)
        pd3dCommandQueue = queue;

    oExecuteCommandLists(queue, NumCommandLists, ppCommandLists);
}

HRESULT __stdcall DX12Hook::hkResizeBuffers(
    IDXGISwapChain *pSwapChain,
    UINT            BufferCount,
    UINT            Width,
    UINT            Height,
    DXGI_FORMAT     NewFormat,
    UINT            SwapChainFlags
) {
    Logger::Debug("[ResizeBuffers] w: {}, h: {}", Width, Height);

    if (GuiOverlay::sInitialized) {
        pd3d11DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
        ImGui_ImplDX11_InvalidateDeviceObjects();
        CleanupRenderTargetResources();
        pd3d11DeviceContext->Flush();
    } else {
        CleanupRenderTargetResources();
    }
    WaitForGPU();

    HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
    if (FAILED(hr)) {
        Logger::Error("[DX12Hook] oResizeBuffers failed with HRESULT: {:#x}", (uint32_t)hr);
        return hr;
    }
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)desc.BufferDesc.Width, (float)desc.BufferDesc.Height);
    CreateRenderTargetResources(pSwapChain);
    if (GuiOverlay::sInitialized) {
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    return hr;
}

HRESULT __stdcall DX12Hook::hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags) {
    // 初始化阶段
    if (!GuiOverlay::sInitialized) {
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        desc.OutputWindow = moduleInfo::gMainWindow = FindChildWindowByClass(moduleInfo::gMainWindow, L"Windows.UI.Core.CoreWindow");

        if (FAILED(pSwapChain->GetDevice(IID_PPV_ARGS(&pd3dDevice))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (!pd3dCommandQueue)
            return oPresent12(pSwapChain, SyncInterval, Flags);

        UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef SPHR_DEBUG
        d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        ID3D12CommandQueue *ppCommandQueues[] = {pd3dCommandQueue};
        if (FAILED(D3D11On12CreateDevice(
                pd3dDevice,
                d3d11DeviceFlags,
                nullptr,
                0,
                reinterpret_cast<IUnknown **>(ppCommandQueues),
                1,
                0,
                &pd3d11Device,
                &pd3d11DeviceContext,
                nullptr
            ))) return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(pd3d11Device->QueryInterface(IID_PPV_ARGS(&pd3d11On12Device))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        try {
            CoreWindow coreWindow = CoreWindow::GetForCurrentThread();
            if (coreWindow) {
                GuiOverlay::initInputManager(std::make_unique<InputManager>(coreWindow));
            } else {
                Logger::Warn("[DX12Hook] CoreWindow::GetForCurrentThread() 返回 nullptr。可能不在 UI 线程?");
                // todo: 回退方案
            }
        } catch (const winrt::hresult_error &ex) {
            Logger::ErrorBox(L"[DX12Hook] 获取 CoreWindow 失败: {} ({:X})", ex.message().c_str(), (int32_t)ex.code());
        }

        GuiOverlay::initImGui(moduleInfo::gMainWindow, pd3d11Device, pd3d11DeviceContext, desc);

        if (!CreateRenderTargetResources(pSwapChain))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        GuiOverlay::sInitialized = true;
    }

    GuiOverlay::refreshCursorPos();
    GuiOverlay::handleHotkey();
    GuiOverlay::saveConfig();

    ImGui_ImplDX11_NewFrame();
    // ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 绘制窗口
    GuiOverlay::frame();

    ImGui::Render();

    const UINT backBufferIdx = pSwapChain->GetCurrentBackBufferIndex();

    pd3d11On12Device->AcquireWrappedResources(&pd3d11WrappedBackBuffer[backBufferIdx], 1);
    pd3d11DeviceContext->OMSetRenderTargets(1, &pd3d11RenderTargetView[backBufferIdx], nullptr);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    pd3d11On12Device->ReleaseWrappedResources(&pd3d11WrappedBackBuffer[backBufferIdx], 1);

    pd3d11DeviceContext->Flush();

    return oPresent12(pSwapChain, SyncInterval, Flags);
}

void DX12Hook::installAsync() {
    std::thread{[]() {
        if (!DX12Hook::install()) {
            Logger::Error("[core] DX12 Hook 安装失败！");
            Logger::ErrorBox(L"DX12 Hook 安装失败！");
        }
    }}.detach();
}

bool DX12Hook::install() {
    Logger::Debug("DX12Hook::install");
    winrt::init_apartment(winrt::apartment_type::multi_threaded);
    return kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success
        && kiero::bind(140, (void **)&oPresent12, hkPresent12) == kiero::Status::Success
        && kiero::bind(54, (void **)&oExecuteCommandLists, hkExecuteCommandLists) == kiero::Status::Success
        && kiero::bind(145, (void **)&oResizeBuffers, hkResizeBuffers) == kiero::Status::Success;
}

void DX12Hook::uninstall() {
    Logger::Debug("DX12Hook::uninstall");
    GuiOverlay::shutdownImGui();

    WaitForGPU();
    CleanupRenderTargetResources();

    if (pd3d11On12Device) {
        pd3d11On12Device->Release();
        pd3d11On12Device = nullptr;
    }
    if (pd3d11DeviceContext) {
        pd3d11DeviceContext->Release();
        pd3d11DeviceContext = nullptr;
    }
    if (pd3d11Device) {
        pd3d11Device->Release();
        pd3d11Device = nullptr;
    }

    if (pd3dCommandList) {
        pd3dCommandList->Release();
        pd3dCommandList = nullptr;
    }

    if (pd3dCommandAllocator) {
        pd3dCommandAllocator->Release();
        pd3dCommandAllocator = nullptr;
    }

    if (pd3dDevice) {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }

    kiero::shutdown();
}
