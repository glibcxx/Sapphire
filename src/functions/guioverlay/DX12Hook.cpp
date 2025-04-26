#include "DX12Hook.h"

#include <kiero/kiero.h>

#include "SDK/core/Core.h"
#include "GuiOverlay.h"

static const int APP_NUM_BACK_BUFFERS = 3;

typedef HRESULT(__stdcall *Present12T)(IDXGISwapChain3 *, UINT, UINT);
typedef void(APIENTRY *ExecuteCommandLists)(ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists);
typedef HRESULT(STDMETHODCALLTYPE *ResizeBuffers_t)(
    IDXGISwapChain *This,
    UINT            BufferCount,
    UINT            Width,
    UINT            Height,
    DXGI_FORMAT     NewFormat,
    UINT            SwapChainFlags);

// Global Variables
WNDPROC             oWndProc = nullptr;
Present12T          oPresent12 = nullptr;
ExecuteCommandLists oExecuteCommandLists = nullptr;
ResizeBuffers_t     oResizeBuffers = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static ID3D12Device *g_pd3dDevice = nullptr;

static ID3D12GraphicsCommandList *g_pd3dCommandList = nullptr;
static ID3D12CommandAllocator    *g_pd3dCommandAllocator = nullptr;
static ID3D12CommandQueue        *g_commandQueue = nullptr;

static ID3D12DescriptorHeap *g_pd3dRtvDescHeap = nullptr;
static ID3D12DescriptorHeap *g_pd3dSrvDescHeap = nullptr;

static ID3D12Resource *g_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};

// Helper Functions
void CreateRenderTarget(IDXGISwapChain *pSwapChain) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = desc.BufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE};
    g_pd3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_pd3dRtvDescHeap));

    const SIZE_T                rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < desc.BufferCount; ++i) {
        pSwapChain->GetBuffer(i, IID_PPV_ARGS(&g_mainRenderTargetResource[i]));
        g_pd3dDevice->CreateRenderTargetView(g_mainRenderTargetResource[i], nullptr, rtvHandle);
        rtvHandle.ptr += rtvDescriptorSize;
    }
}

void CleanupRenderTarget(IDXGISwapChain *pSwapChain) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    const UINT bufferCount = desc.BufferCount;

    for (UINT i = 0; i < bufferCount; ++i) {
        if (g_mainRenderTargetResource[i]) {
            g_mainRenderTargetResource[i]->Release();
            g_mainRenderTargetResource[i] = nullptr;
        }
    }
}

void WaitForGPU() {
    ID3D12Fence *fence;
    g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    const UINT64 fenceValue = 1;
    g_commandQueue->Signal(fence, fenceValue);
    if (fence->GetCompletedValue() < fenceValue) {
        HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        fence->SetEventOnCompletion(fenceValue, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }
    fence->Release();
}

HWND FindChildWindowByClass(HWND parent, const wchar_t *className) {
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

bool IsGameCapturingMouse() {
    CURSORINFO ci = {sizeof(ci)};
    GetCursorInfo(&ci);
    return (ci.flags == 0);
}

LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    if (uMsg == WM_INPUT) {
        RAWINPUT raw;
        UINT     size = sizeof(raw);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER));

        if (IsGameCapturingMouse() && raw.header.dwType == RIM_TYPEMOUSE) {
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDelta.x += raw.data.mouse.lLastX;
            io.MouseDelta.y += raw.data.mouse.lLastY;
        }
    }
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void hkExecuteCommandLists(ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists) {
    if (!g_commandQueue)
        g_commandQueue = queue;

    oExecuteCommandLists(queue, NumCommandLists, ppCommandLists);
}

HRESULT STDMETHODCALLTYPE hkResizeBuffers(
    IDXGISwapChain *pSwapChain,
    UINT            BufferCount,
    UINT            Width,
    UINT            Height,
    DXGI_FORMAT     NewFormat,
    UINT            SwapChainFlags) {
    WaitForGPU();
    CleanupRenderTarget(pSwapChain);
    if (g_pd3dRtvDescHeap) {
        g_pd3dRtvDescHeap->Release();
        g_pd3dRtvDescHeap = nullptr;
    }

    HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
    if (FAILED(hr)) return hr;

    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);
    CreateRenderTarget(pSwapChain);

    return hr;
}

static HHOOK g_hMouseHook = nullptr;
static HHOOK g_hKeyboardHook = nullptr;

// 鼠标钩子
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // if (nCode >= HC_ACTION)
    // {
    //     MSLLHOOKSTRUCT *pMouse = (MSLLHOOKSTRUCT *)lParam;
    //     // 处理按钮事件
    //     ImGuiIO &io = ImGui::GetIO();
    //     switch (wParam)
    //     {
    //     case WM_LBUTTONDOWN: io.MouseDown[0] = true; break;
    //     case WM_LBUTTONUP: io.MouseDown[0] = false; break;
    //     case WM_RBUTTONDOWN: io.MouseDown[1] = true; break;
    //     case WM_RBUTTONUP: io.MouseDown[1] = false; break;
    //     case WM_MBUTTONDOWN: io.MouseDown[2] = true; break;
    //     case WM_MBUTTONUP: io.MouseDown[2] = false; break;
    //     case WM_MOUSEWHEEL:
    //         io.MouseWheel += GET_WHEEL_DELTA_WPARAM(pMouse->mouseData) / (float)WHEEL_DELTA;
    //         break;
    //     case WM_MOUSEHWHEEL:
    //         io.MouseWheelH += GET_WHEEL_DELTA_WPARAM(pMouse->mouseData) / (float)WHEEL_DELTA;
    //         break;
    //     }
    // }
    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

ImGuiKey ImGui_ImplWin32_KeyEventToImGuiKey(WPARAM wParam, LPARAM lParam);

// 键盘钩子
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= HC_ACTION) {
        KBDLLHOOKSTRUCT *pKey = (KBDLLHOOKSTRUCT *)lParam;
        ImGuiIO         &io = ImGui::GetIO();
        const ImGuiKey   key = ImGui_ImplWin32_KeyEventToImGuiKey(pKey->vkCode, lParam);
        if (key != ImGuiKey_None) {
            const bool is_key_down = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);

            // 添加按键事件
            io.AddKeyEvent(key, is_key_down);

            io.AddKeyEvent(ImGuiKey_ModCtrl, (GetAsyncKeyState(VK_CONTROL) & 0x8000));
            io.AddKeyEvent(ImGuiKey_ModShift, (GetAsyncKeyState(VK_SHIFT) & 0x8000));
            io.AddKeyEvent(ImGuiKey_ModAlt, (GetAsyncKeyState(VK_MENU) & 0x8000));
        }

        if (wParam == WM_KEYDOWN && io.WantTextInput) {
            BYTE keyboardState[256] = {0};
            GetKeyboardState(keyboardState);
            WCHAR charBuf[2] = {0};
            if (ToUnicode(pKey->vkCode, pKey->scanCode, keyboardState, charBuf, 1, 0) > 0) {
                io.AddInputCharacterUTF16(charBuf[0]);
            }
        }

        // 阻止事件传递
        if (io.WantCaptureKeyboard) return 1;
    }
    return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

HRESULT __stdcall hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags) {
    // 初始化阶段
    if (!GuiOverlay::sInitialized) {
        // SetThreadDescription(GetCurrentThread(), L"hkPresent12 Thread!!!!"); // 方便调试

        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        desc.OutputWindow = moduleInfo::gMainWindow = FindChildWindowByClass(moduleInfo::gMainWindow, L"Windows.UI.Core.CoreWindow");

        // g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), 0); // 有bug，以后再解决
        g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

        if (FAILED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_pd3dCommandAllocator))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                   g_pd3dCommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList))))
            return oPresent12(pSwapChain, SyncInterval, Flags);
        g_pd3dCommandList->Close();

        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.NumDescriptors = APP_NUM_BACK_BUFFERS;
        if (FAILED(g_pd3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_pd3dRtvDescHeap))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.NumDescriptors = APP_NUM_BACK_BUFFERS;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (FAILED(g_pd3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&g_pd3dSrvDescHeap))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        GuiOverlay::initImGui(moduleInfo::gMainWindow, g_pd3dDevice, g_pd3dSrvDescHeap, APP_NUM_BACK_BUFFERS);

        CreateRenderTarget(pSwapChain);

        // Hook内容窗口的窗口过程
        oWndProc = (WNDPROC)SetWindowLongPtr(moduleInfo::gMainWindow, GWLP_WNDPROC, (LONG_PTR)hkWndProc);
        GuiOverlay::sInitialized = true;
    }

    GuiOverlay::handleKeyPress();

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 绘制窗口
    if (GuiOverlay::sShowOverlay)
        GuiOverlay::drawMainOverlay();

    const UINT backBufferIdx = pSwapChain->GetCurrentBackBufferIndex();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    g_pd3dCommandAllocator->Reset();
    g_pd3dCommandList->Reset(g_pd3dCommandAllocator, nullptr);
    g_pd3dCommandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
        g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIdx * g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    g_pd3dCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);

    ImGui::Render();

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    g_pd3dCommandList->ResourceBarrier(1, &barrier);
    g_pd3dCommandList->Close();

    g_commandQueue->ExecuteCommandLists(1, (ID3D12CommandList *const *)&g_pd3dCommandList);

    return oPresent12(pSwapChain, SyncInterval, Flags);
}

bool InstallDX12Hook() {
    return kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success
        && kiero::bind(140, (void **)&oPresent12, hkPresent12) == kiero::Status::Success
        && kiero::bind(54, (void **)&oExecuteCommandLists, hkExecuteCommandLists) == kiero::Status::Success
        && kiero::bind(145, (void **)&oResizeBuffers, hkResizeBuffers) == kiero::Status::Success;
}

void UninstallDX12Hook() {
    LogBox::Info(L"Uninstalling DX12 Hook...");
    GuiOverlay::shutdownImGui();

    if (g_hMouseHook) {
        UnhookWindowsHookEx(g_hMouseHook);
        g_hMouseHook = nullptr;
    }

    if (g_hKeyboardHook) {
        UnhookWindowsHookEx(g_hKeyboardHook);
        g_hKeyboardHook = nullptr;
    }

    if (g_pd3dCommandList) {
        g_pd3dCommandList->Release();
        g_pd3dCommandList = nullptr;
    }

    if (g_pd3dCommandAllocator) {
        g_pd3dCommandAllocator->Release();
        g_pd3dCommandAllocator = nullptr;
    }

    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }

    if (g_pd3dRtvDescHeap) {
        g_pd3dRtvDescHeap->Release();
        g_pd3dRtvDescHeap = nullptr;
    }

    if (g_pd3dSrvDescHeap) {
        g_pd3dSrvDescHeap->Release();
        g_pd3dSrvDescHeap = nullptr;
    }

    kiero::shutdown();
}
