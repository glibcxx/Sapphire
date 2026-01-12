#include "RenderBackend.h"

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.Foundation.h>

#include <kiero/kiero.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/input/InputManager.h"

#include "Runtime.h"
#include "SDK/api/sapphire/platform/Environment.h"

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace sapphire::core {

    static RenderBackend *backend;
    static WNDPROC        oWndProc = nullptr;

    static LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;
        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    RenderBackend::RenderBackend() {
        backend = this;
        std::thread{[this]() {
            if (!this->_init()) {
                sapphire::error("RenderBackend: DX12 Hook 安装失败！");
                sapphire::alert(L"RenderBackend: DX12 Hook 安装失败！");
            }
        }}.detach();
    }

    bool RenderBackend::_init() {
        sapphire::debug("RenderBackend: RenderBackend::_init");
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
        return kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success
            && kiero::bind(140, (void **)&mPresent12, (void *)hkPresent12) == kiero::Status::Success
            && kiero::bind(54, (void **)&mExecuteCommandLists, (void *)hkExecuteCommandLists) == kiero::Status::Success
            && kiero::bind(145, (void **)&mResizeBuffers, (void *)hkResizeBuffers) == kiero::Status::Success;
    }

    RenderBackend::~RenderBackend() {
        RenderBackend::_shutdown();
    }

    void RenderBackend::_shutdown() {
        sapphire::debug("RenderBackend: RenderBackend::_shutdown");
        ui::GuiOverlay::shutdownImGui();

        this->WaitForGPU();
        this->CleanupRenderTargetResources();

        if (mD3d11On12Device) {
            mD3d11On12Device->Release();
            mD3d11On12Device = nullptr;
        }
        if (mD3d11DeviceContext) {
            mD3d11DeviceContext->Release();
            mD3d11DeviceContext = nullptr;
        }
        if (mD3d11Device) {
            mD3d11Device->Release();
            mD3d11Device = nullptr;
        }

        if (mD3dCommandList) {
            mD3dCommandList->Release();
            mD3dCommandList = nullptr;
        }

        if (mD3dCommandAllocator) {
            mD3dCommandAllocator->Release();
            mD3dCommandAllocator = nullptr;
        }

        if (mD3dDevice) {
            mD3dDevice->Release();
            mD3dDevice = nullptr;
        }

        kiero::shutdown();
    }

    HRESULT __stdcall RenderBackend::hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags) {
        // 初始化阶段
        auto &env = sapphire::platform::Environment::getInstance();
        if (!ui::GuiOverlay::sInitialized) {
            DXGI_SWAP_CHAIN_DESC desc;
            pSwapChain->GetDesc(&desc);
            desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
            desc.OutputWindow = backend->FindChildWindowByClass(env.getMainWindow(), L"Windows.UI.Core.CoreWindow");
            env.setMainWindow(desc.OutputWindow);

            if (FAILED(pSwapChain->GetDevice(IID_PPV_ARGS(&backend->mD3dDevice))))
                return backend->mPresent12(pSwapChain, SyncInterval, Flags);

            if (!backend->mD3dCommandQueue)
                return backend->mPresent12(pSwapChain, SyncInterval, Flags);

            UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef SPHR_DEBUG
            d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
            ID3D12CommandQueue *ppCommandQueues[] = {backend->mD3dCommandQueue};
            if (FAILED(D3D11On12CreateDevice(
                    backend->mD3dDevice,
                    d3d11DeviceFlags,
                    nullptr,
                    0,
                    reinterpret_cast<IUnknown **>(ppCommandQueues),
                    1,
                    0,
                    &backend->mD3d11Device,
                    &backend->mD3d11DeviceContext,
                    nullptr
                ))) return backend->mPresent12(pSwapChain, SyncInterval, Flags);

            if (FAILED(backend->mD3d11Device->QueryInterface(IID_PPV_ARGS(&backend->mD3d11On12Device))))
                return backend->mPresent12(pSwapChain, SyncInterval, Flags);

            try {
                CoreWindow coreWindow = CoreWindow::GetForCurrentThread();
                if (coreWindow) {
                    sapphire::input::InputManager::getInstance().init(coreWindow);
                } else {
                    sapphire::warn("RenderBackend: CoreWindow::GetForCurrentThread() 返回 nullptr。可能不在 UI 线程?");
                    // todo: 回退方案
                }
            } catch (const winrt::hresult_error &ex) {
                sapphire::alert(
                    L"RenderBackend: 获取 CoreWindow 失败: {} ({:X})", ex.message().c_str(), (int32_t)ex.code()
                );
            }

            ui::GuiOverlay::initImGui(env.getMainWindow(), backend->mD3d11Device, backend->mD3d11DeviceContext, desc);

            if (!backend->CreateRenderTargetResources(pSwapChain))
                return backend->mPresent12(pSwapChain, SyncInterval, Flags);

            oWndProc = (WNDPROC)SetWindowLongPtr(env.getMainWindow(), GWLP_WNDPROC, (LONG_PTR)hkWndProc);

            ui::GuiOverlay::sInitialized = true;
        }

        ui::GuiOverlay::refreshCursorPos();
        ui::GuiOverlay::handleHotkey();
        ui::GuiOverlay::saveConfig();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 绘制窗口
        ui::GuiOverlay::frame();

        ImGui::Render();

        const UINT backBufferIdx = pSwapChain->GetCurrentBackBufferIndex();

        backend->mD3d11On12Device->AcquireWrappedResources(&backend->mD3d11WrappedBackBuffer[backBufferIdx], 1);
        backend->mD3d11DeviceContext->OMSetRenderTargets(1, &backend->mD3d11RenderTargetView[backBufferIdx], nullptr);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        backend->mD3d11On12Device->ReleaseWrappedResources(&backend->mD3d11WrappedBackBuffer[backBufferIdx], 1);

        backend->mD3d11DeviceContext->Flush();

        return backend->mPresent12(pSwapChain, SyncInterval, Flags);
    }

    bool RenderBackend::CreateRenderTargetResources(IDXGISwapChain *pSwapChain) {
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        const UINT bufferCount = desc.BufferCount > APP_NUM_BACK_BUFFERS ? APP_NUM_BACK_BUFFERS : desc.BufferCount;

        for (UINT i = 0; i < bufferCount; ++i) {
            if (FAILED(pSwapChain->GetBuffer(i, IID_PPV_ARGS(&mD3dMainRenderTargetResource[i]))))
                return false;

            D3D11_RESOURCE_FLAGS d3d11Flags = {D3D11_BIND_RENDER_TARGET};
            if (FAILED(mD3d11On12Device->CreateWrappedResource(
                    mD3dMainRenderTargetResource[i],
                    &d3d11Flags,
                    D3D12_RESOURCE_STATE_RENDER_TARGET,
                    D3D12_RESOURCE_STATE_PRESENT,
                    IID_PPV_ARGS(&mD3d11WrappedBackBuffer[i])
                ))) return false;

            if (FAILED(mD3d11Device->CreateRenderTargetView(
                    mD3d11WrappedBackBuffer[i], nullptr, &mD3d11RenderTargetView[i]
                )))
                return false;
        }
        return true;
    }

    void RenderBackend::CleanupRenderTargetResources() {
        for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; ++i) {
            if (mD3d11RenderTargetView[i]) {
                mD3d11RenderTargetView[i]->Release();
                mD3d11RenderTargetView[i] = nullptr;
            }
            if (mD3d11WrappedBackBuffer[i]) {
                mD3d11WrappedBackBuffer[i]->Release();
                mD3d11WrappedBackBuffer[i] = nullptr;
            }
            if (mD3dMainRenderTargetResource[i]) {
                mD3dMainRenderTargetResource[i]->Release();
                mD3dMainRenderTargetResource[i] = nullptr;
            }
        }
    }

    void RenderBackend::WaitForGPU() {
        if (!mD3dDevice && !mD3dCommandQueue)
            return;
        ID3D12Fence *fence;
        if (SUCCEEDED(mD3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
            const UINT64 fenceValue = 1;
            mD3dCommandQueue->Signal(fence, fenceValue);
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

    HWND RenderBackend::FindChildWindowByClass(HWND parent, const wchar_t *className) {
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

    void __stdcall RenderBackend::hkExecuteCommandLists(
        ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists
    ) {
        if (!backend->mD3dCommandQueue)
            backend->mD3dCommandQueue = queue;

        backend->mExecuteCommandLists(queue, NumCommandLists, ppCommandLists);
    }

    HRESULT __stdcall RenderBackend::hkResizeBuffers(
        IDXGISwapChain *pSwapChain,
        UINT            BufferCount,
        UINT            Width,
        UINT            Height,
        DXGI_FORMAT     NewFormat,
        UINT            SwapChainFlags
    ) {
        sapphire::debug("RenderBackend: hkResizeBuffers -> w: {}, h: {}", Width, Height);

        if (ui::GuiOverlay::sInitialized) {
            backend->mD3d11DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
            ImGui_ImplDX11_InvalidateDeviceObjects();
            backend->CleanupRenderTargetResources();
            backend->mD3d11DeviceContext->Flush();
        } else {
            backend->CleanupRenderTargetResources();
        }
        backend->WaitForGPU();

        HRESULT hr = backend->mResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
        if (FAILED(hr)) {
            sapphire::error("RenderBackend: oResizeBuffers failed with HRESULT: {:#x}", (uint32_t)hr);
            return hr;
        }
        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)desc.BufferDesc.Width, (float)desc.BufferDesc.Height);
        backend->CreateRenderTargetResources(pSwapChain);
        if (ui::GuiOverlay::sInitialized) {
            ImGui_ImplDX11_CreateDeviceObjects();
        }

        return hr;
    }

} // namespace sapphire::core