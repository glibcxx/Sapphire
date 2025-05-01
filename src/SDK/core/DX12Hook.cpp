#include "DX12Hook.h"

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.Foundation.h>

#include <kiero/kiero.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>

#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/input/InputManager.h"

#include "Core.h"

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;

void DX12Hook::CreateRenderTarget(IDXGISwapChain *pSwapChain) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = desc.BufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
    };
    pd3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&pd3dRtvDescHeap));

    const SIZE_T                rtvDescriptorSize = pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < desc.BufferCount; ++i) {
        pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pd3dMainRenderTargetResource[i]));
        pd3dDevice->CreateRenderTargetView(pd3dMainRenderTargetResource[i], nullptr, rtvHandle);
        rtvHandle.ptr += rtvDescriptorSize;
    }
}

void DX12Hook::CleanupRenderTarget(IDXGISwapChain *pSwapChain) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    const UINT bufferCount = desc.BufferCount;

    for (UINT i = 0; i < bufferCount; ++i) {
        if (pd3dMainRenderTargetResource[i]) {
            pd3dMainRenderTargetResource[i]->Release();
            pd3dMainRenderTargetResource[i] = nullptr;
        }
    }
}

void DX12Hook::WaitForGPU() {
    ID3D12Fence *fence;
    pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    const UINT64 fenceValue = 1;
    pd3dCommandQueue->Signal(fence, fenceValue);
    if (fence->GetCompletedValue() < fenceValue) {
        HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        fence->SetEventOnCompletion(fenceValue, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }
    fence->Release();
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
    WaitForGPU();
    CleanupRenderTarget(pSwapChain);
    if (pd3dRtvDescHeap) {
        pd3dRtvDescHeap->Release();
        pd3dRtvDescHeap = nullptr;
    }

    HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
    if (FAILED(hr)) return hr;

    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)desc.BufferDesc.Width, (float)desc.BufferDesc.Height);

    CreateRenderTarget(pSwapChain);

    return hr;
}

HRESULT __stdcall DX12Hook::hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags) {
    // 初始化阶段
    if (!GuiOverlay::sInitialized) {
        // SetThreadDescription(GetCurrentThread(), L"hkPresent12 Thread!!!!"); // 方便调试

        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        desc.OutputWindow = moduleInfo::gMainWindow = FindChildWindowByClass(moduleInfo::gMainWindow, L"Windows.UI.Core.CoreWindow");

        if (FAILED(pSwapChain->GetDevice(IID_PPV_ARGS(&pd3dDevice))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pd3dCommandAllocator))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pd3dCommandAllocator, nullptr, IID_PPV_ARGS(&pd3dCommandList))))
            return oPresent12(pSwapChain, SyncInterval, Flags);
        pd3dCommandList->Close();

        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.NumDescriptors = APP_NUM_BACK_BUFFERS;
        if (FAILED(pd3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&pd3dRtvDescHeap))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.NumDescriptors = APP_NUM_BACK_BUFFERS;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (FAILED(pd3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&pd3dSrvDescHeap))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        try {
            CoreWindow coreWindow = CoreWindow::GetForCurrentThread();
            if (coreWindow) {
                GuiOverlay::initInputManager(std::make_unique<InputManager>(coreWindow));
            } else {
                Logger::WarnBox(L"CoreWindow::GetForCurrentThread() 返回 nullptr。可能不在 UI 线程?");
                // todo: 回退方案
            }
        } catch (const winrt::hresult_error &ex) {
            Logger::ErrorBox(L"获取 CoreWindow 失败: {} ({:X})", ex.message().c_str(), (int32_t)ex.code());
        }

        GuiOverlay::initImGui(moduleInfo::gMainWindow, pd3dDevice, pd3dSrvDescHeap, desc, APP_NUM_BACK_BUFFERS);

        CreateRenderTarget(pSwapChain);

        GuiOverlay::sInitialized = true;
    }

    GuiOverlay::refreshCursorPos();
    GuiOverlay::handleHotkey();

    ImGui_ImplDX12_NewFrame();
    // ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 绘制窗口
    GuiOverlay::drawGUI();

    const UINT backBufferIdx = pSwapChain->GetCurrentBackBufferIndex();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = pd3dMainRenderTargetResource[backBufferIdx];
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    pd3dCommandAllocator->Reset();
    pd3dCommandList->Reset(pd3dCommandAllocator, nullptr);
    pd3dCommandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
        pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIdx * pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    pd3dCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    pd3dCommandList->SetDescriptorHeaps(1, &pd3dSrvDescHeap);

    ImGui::Render();

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pd3dCommandList);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    pd3dCommandList->ResourceBarrier(1, &barrier);
    pd3dCommandList->Close();

    pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList *const *)&pd3dCommandList);

    return oPresent12(pSwapChain, SyncInterval, Flags);
}

bool DX12Hook::install() {
    winrt::init_apartment(winrt::apartment_type::multi_threaded);
    return kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success
        && kiero::bind(140, (void **)&oPresent12, hkPresent12) == kiero::Status::Success
        && kiero::bind(54, (void **)&oExecuteCommandLists, hkExecuteCommandLists) == kiero::Status::Success
        && kiero::bind(145, (void **)&oResizeBuffers, hkResizeBuffers) == kiero::Status::Success;
}

void DX12Hook::uninstall() {
    GuiOverlay::shutdownImGui();

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

    if (pd3dRtvDescHeap) {
        pd3dRtvDescHeap->Release();
        pd3dRtvDescHeap = nullptr;
    }

    if (pd3dSrvDescHeap) {
        pd3dSrvDescHeap->Release();
        pd3dSrvDescHeap = nullptr;
    }

    kiero::shutdown();

    winrt::uninit_apartment();
}
