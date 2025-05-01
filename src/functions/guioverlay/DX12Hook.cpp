#include "DX12Hook.h"

#include <Windows.h>

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.Foundation.h>

#include <kiero/kiero.h>

#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-deps/Input/MouseDevice.h"

#include "GuiOverlay.h"

#include "../builtinPlugin.h"

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;

static const int APP_NUM_BACK_BUFFERS = 3;

typedef HRESULT(__stdcall *Present12T)(IDXGISwapChain3 *, UINT, UINT);
typedef void(APIENTRY *ExecuteCommandLists)(
    ID3D12CommandQueue       *queue,
    UINT                      NumCommandLists,
    ID3D12CommandList *const *ppCommandLists
);
typedef HRESULT(STDMETHODCALLTYPE *ResizeBuffers_t)(
    IDXGISwapChain *This,
    UINT            BufferCount,
    UINT            Width,
    UINT            Height,
    DXGI_FORMAT     NewFormat,
    UINT            SwapChainFlags
);

// Global Variables
Present12T          oPresent12 = nullptr;
ExecuteCommandLists oExecuteCommandLists = nullptr;
ResizeBuffers_t     oResizeBuffers = nullptr;

static ID3D12Device *g_pd3dDevice = nullptr;

static ID3D12GraphicsCommandList *g_pd3dCommandList = nullptr;
static ID3D12CommandAllocator    *g_pd3dCommandAllocator = nullptr;
static ID3D12CommandQueue        *g_commandQueue = nullptr;

static ID3D12DescriptorHeap *g_pd3dRtvDescHeap = nullptr;
static ID3D12DescriptorHeap *g_pd3dSrvDescHeap = nullptr;

static ID3D12Resource *g_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};

CoreWindow g_coreWindow{nullptr};

CoreWindow::PointerPressed_revoker  g_pointerPressedRevoker;
CoreWindow::PointerReleased_revoker g_pointerReleasedRevoker;
// CoreWindow::PointerMoved_revoker        g_pointerMovedRevoker;
CoreWindow::PointerWheelChanged_revoker g_pointerWheelRevoker;
CoreWindow::Activated_revoker           g_activatedRevoker;

CoreDispatcher                                  g_coreDispatcher{nullptr};
CoreDispatcher::AcceleratorKeyActivated_revoker g_acceleratorRevoker;

std::atomic<bool> g_wantCaptureMouse{false};
std::atomic<bool> g_wantCaptureKeyboard{false};

ImGuiKey KeyEventToImGuiKey(WPARAM wParam) {
    switch (wParam) {
    case VK_TAB: return ImGuiKey_Tab;
    case VK_LEFT: return ImGuiKey_LeftArrow;
    case VK_RIGHT: return ImGuiKey_RightArrow;
    case VK_UP: return ImGuiKey_UpArrow;
    case VK_DOWN: return ImGuiKey_DownArrow;
    case VK_PRIOR: return ImGuiKey_PageUp;
    case VK_NEXT: return ImGuiKey_PageDown;
    case VK_HOME: return ImGuiKey_Home;
    case VK_END: return ImGuiKey_End;
    case VK_INSERT: return ImGuiKey_Insert;
    case VK_DELETE: return ImGuiKey_Delete;
    case VK_BACK: return ImGuiKey_Backspace;
    case VK_SPACE: return ImGuiKey_Space;
    case VK_RETURN: return ImGuiKey_Enter;
    case VK_ESCAPE: return ImGuiKey_Escape;
    case VK_OEM_7: return ImGuiKey_Apostrophe;
    case VK_OEM_COMMA: return ImGuiKey_Comma;
    case VK_OEM_MINUS: return ImGuiKey_Minus;
    case VK_OEM_PERIOD: return ImGuiKey_Period;
    case VK_OEM_2: return ImGuiKey_Slash;
    case VK_OEM_1: return ImGuiKey_Semicolon;
    case VK_OEM_PLUS: return ImGuiKey_Equal;
    case VK_OEM_4: return ImGuiKey_LeftBracket;
    case VK_OEM_5: return ImGuiKey_Backslash;
    case VK_OEM_6: return ImGuiKey_RightBracket;
    case VK_OEM_3: return ImGuiKey_GraveAccent;
    case VK_CAPITAL: return ImGuiKey_CapsLock;
    case VK_SCROLL: return ImGuiKey_ScrollLock;
    case VK_NUMLOCK: return ImGuiKey_NumLock;
    case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
    case VK_PAUSE: return ImGuiKey_Pause;
    case VK_NUMPAD0: return ImGuiKey_Keypad0;
    case VK_NUMPAD1: return ImGuiKey_Keypad1;
    case VK_NUMPAD2: return ImGuiKey_Keypad2;
    case VK_NUMPAD3: return ImGuiKey_Keypad3;
    case VK_NUMPAD4: return ImGuiKey_Keypad4;
    case VK_NUMPAD5: return ImGuiKey_Keypad5;
    case VK_NUMPAD6: return ImGuiKey_Keypad6;
    case VK_NUMPAD7: return ImGuiKey_Keypad7;
    case VK_NUMPAD8: return ImGuiKey_Keypad8;
    case VK_NUMPAD9: return ImGuiKey_Keypad9;
    case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
    case VK_DIVIDE: return ImGuiKey_KeypadDivide;
    case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case VK_ADD: return ImGuiKey_KeypadAdd;
    case VK_LSHIFT: return ImGuiKey_LeftShift;
    case VK_LCONTROL: return ImGuiKey_LeftCtrl;
    case VK_LMENU: return ImGuiKey_LeftAlt;
    case VK_LWIN: return ImGuiKey_LeftSuper;
    case VK_RSHIFT: return ImGuiKey_RightShift;
    case VK_RCONTROL: return ImGuiKey_RightCtrl;
    case VK_RMENU: return ImGuiKey_RightAlt;
    case VK_RWIN: return ImGuiKey_RightSuper;
    case VK_APPS: return ImGuiKey_Menu;
    case '0': return ImGuiKey_0;
    case '1': return ImGuiKey_1;
    case '2': return ImGuiKey_2;
    case '3': return ImGuiKey_3;
    case '4': return ImGuiKey_4;
    case '5': return ImGuiKey_5;
    case '6': return ImGuiKey_6;
    case '7': return ImGuiKey_7;
    case '8': return ImGuiKey_8;
    case '9': return ImGuiKey_9;
    case 'A': return ImGuiKey_A;
    case 'B': return ImGuiKey_B;
    case 'C': return ImGuiKey_C;
    case 'D': return ImGuiKey_D;
    case 'E': return ImGuiKey_E;
    case 'F': return ImGuiKey_F;
    case 'G': return ImGuiKey_G;
    case 'H': return ImGuiKey_H;
    case 'I': return ImGuiKey_I;
    case 'J': return ImGuiKey_J;
    case 'K': return ImGuiKey_K;
    case 'L': return ImGuiKey_L;
    case 'M': return ImGuiKey_M;
    case 'N': return ImGuiKey_N;
    case 'O': return ImGuiKey_O;
    case 'P': return ImGuiKey_P;
    case 'Q': return ImGuiKey_Q;
    case 'R': return ImGuiKey_R;
    case 'S': return ImGuiKey_S;
    case 'T': return ImGuiKey_T;
    case 'U': return ImGuiKey_U;
    case 'V': return ImGuiKey_V;
    case 'W': return ImGuiKey_W;
    case 'X': return ImGuiKey_X;
    case 'Y': return ImGuiKey_Y;
    case 'Z': return ImGuiKey_Z;
    case VK_F1: return ImGuiKey_F1;
    case VK_F2: return ImGuiKey_F2;
    case VK_F3: return ImGuiKey_F3;
    case VK_F4: return ImGuiKey_F4;
    case VK_F5: return ImGuiKey_F5;
    case VK_F6: return ImGuiKey_F6;
    case VK_F7: return ImGuiKey_F7;
    case VK_F8: return ImGuiKey_F8;
    case VK_F9: return ImGuiKey_F9;
    case VK_F10: return ImGuiKey_F10;
    case VK_F11: return ImGuiKey_F11;
    case VK_F12: return ImGuiKey_F12;
    case VK_F13: return ImGuiKey_F13;
    case VK_F14: return ImGuiKey_F14;
    case VK_F15: return ImGuiKey_F15;
    case VK_F16: return ImGuiKey_F16;
    case VK_F17: return ImGuiKey_F17;
    case VK_F18: return ImGuiKey_F18;
    case VK_F19: return ImGuiKey_F19;
    case VK_F20: return ImGuiKey_F20;
    case VK_F21: return ImGuiKey_F21;
    case VK_F22: return ImGuiKey_F22;
    case VK_F23: return ImGuiKey_F23;
    case VK_F24: return ImGuiKey_F24;
    case VK_BROWSER_BACK: return ImGuiKey_AppBack;
    case VK_BROWSER_FORWARD: return ImGuiKey_AppForward;
    default: return ImGuiKey_None;
    }
}

void OnAcceleratorKeyActivated(
    const CoreDispatcher          &sender,
    const AcceleratorKeyEventArgs &args
) {
    winrt::Windows::System::VirtualKey vk = args.VirtualKey();
    CoreAcceleratorKeyEventType        eventType = args.EventType();
    CorePhysicalKeyStatus              keyStatus = args.KeyStatus();

    const bool extendedkey = keyStatus.IsExtendedKey;

    const bool isPressed = eventType == CoreAcceleratorKeyEventType::KeyDown
                        || eventType == CoreAcceleratorKeyEventType::SystemKeyDown;

    ImGuiIO &io = ImGui::GetIO();
    switch (vk) {
    case winrt::Windows::System::VirtualKey::Menu:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_LeftAlt : ImGuiKey::ImGuiKey_RightAlt, isPressed);
        io.AddKeyEvent(ImGuiKey::ImGuiMod_Alt, isPressed);
        break;
    case winrt::Windows::System::VirtualKey::Control:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_LeftCtrl : ImGuiKey::ImGuiKey_RightCtrl, isPressed);
        io.AddKeyEvent(ImGuiKey::ImGuiMod_Ctrl, isPressed);
        break;
    case winrt::Windows::System::VirtualKey::Shift:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_LeftShift : ImGuiKey::ImGuiKey_RightShift, isPressed);
        io.AddKeyEvent(ImGuiKey::ImGuiMod_Shift, isPressed);
        break;
    case winrt::Windows::System::VirtualKey::Delete:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_Delete : ImGuiKey::ImGuiKey_KeypadDecimal, isPressed);
        break;
    default:
        if (ImGuiKey imguiKey = KeyEventToImGuiKey(static_cast<WPARAM>(vk)); imguiKey != ImGuiKey_None) {
            io.AddKeyEvent(imguiKey, isPressed);
        }
        break;
    }

    if (isPressed && io.WantTextInput) {
        BYTE keyboardState[256] = {0};
        GetKeyboardState(keyboardState);
        WCHAR charBuf[2] = {0};
        if (ToUnicode(static_cast<UINT>(vk), keyStatus.ScanCode, keyboardState, charBuf, 1, 0) > 0) {
            io.AddInputCharacterUTF16(charBuf[0]);
        }
    }

    if (g_wantCaptureKeyboard) {
        args.Handled(true);
    }
}

HOOK_TYPE(
    CanncelMouseEvent,
    MouseDevice,
    MouseDevice::feed,
    void,
    MouseAction::ActionType action,
    int                     buttonData,
    short                   x,
    short                   y,
    short                   dx,
    short                   dy,
    uint8_t                 a8
) {
    if (g_wantCaptureMouse)
        return;
    this->origin(action, buttonData, x, y, dx, dy, a8);
}

void OnPointerPressed(const CoreWindow &sender, const PointerEventArgs &args) {
    ImGuiIO                      &io = ImGui::GetIO();
    Input::PointerPoint           point = args.CurrentPoint();
    Input::PointerPointProperties props = point.Properties();

    Input::PointerUpdateKind updateKind = props.PointerUpdateKind();
    int                      button = -1;
    switch (updateKind) {
    case Input::PointerUpdateKind::LeftButtonPressed: button = 0; break;
    case Input::PointerUpdateKind::RightButtonPressed: button = 1; break;
    case Input::PointerUpdateKind::MiddleButtonPressed: button = 2; break;
    case Input::PointerUpdateKind::XButton1Pressed: button = 3; break;
    case Input::PointerUpdateKind::XButton2Pressed: button = 4; break;
    default: break;
    }

    if (button != -1) {
        io.AddMouseButtonEvent(button, true);
    }
    if (g_wantCaptureMouse) {
        args.Handled(true);
    }
}

void OnPointerReleased(const CoreWindow &sender, const PointerEventArgs &args) {
    ImGuiIO                      &io = ImGui::GetIO();
    Input::PointerPoint           point = args.CurrentPoint();
    Input::PointerPointProperties props = point.Properties();

    Input::PointerUpdateKind updateKind = props.PointerUpdateKind();
    int                      button = -1;
    switch (updateKind) {
    case Input::PointerUpdateKind::LeftButtonReleased: button = 0; break;
    case Input::PointerUpdateKind::RightButtonReleased: button = 1; break;
    case Input::PointerUpdateKind::MiddleButtonReleased: button = 2; break;
    case Input::PointerUpdateKind::XButton1Released: button = 3; break;
    case Input::PointerUpdateKind::XButton2Released: button = 4; break;
    default: break;
    }
    if (button != -1) {
        io.AddMouseButtonEvent(button, false);
    }
    if (g_wantCaptureMouse) {
        args.Handled(true);
    }
}

// void OnPointerMoved(const CoreWindow &sender, const PointerEventArgs &args) {
//     ImGuiIO &io = ImGui::GetIO();
//     if (io.WantCaptureMouse) {
//         args.Handled(true);
//     }
// }

void OnPointerWheelChanged(const CoreWindow &sender, const PointerEventArgs &args) {
    using namespace Input;
    ImGuiIO               &io = ImGui::GetIO();
    PointerPoint           point = args.CurrentPoint();
    PointerPointProperties props = point.Properties();

    int32_t wheelDelta = props.MouseWheelDelta();
    if (wheelDelta != 0) {
        float wheelAmount = (float)wheelDelta / (float)WHEEL_DELTA;
        float wheel_y = 0.0f;
        float wheel_x = 0.0f;
        (props.IsHorizontalMouseWheel() ? wheel_x : wheel_y) = wheelAmount;
        io.AddMouseWheelEvent(wheel_x, wheel_y);
    }
    if (g_wantCaptureMouse) {
        args.Handled(true);
    }
}

void SubscribeCoreWindowEvents() {
    if (!g_coreWindow) return;
    try {
        g_pointerPressedRevoker = g_coreWindow.PointerPressed(winrt::auto_revoke, &OnPointerPressed);
        g_pointerReleasedRevoker = g_coreWindow.PointerReleased(winrt::auto_revoke, &OnPointerReleased);
        // g_pointerMovedRevoker = g_coreWindow.PointerMoved(winrt::auto_revoke, &OnPointerMoved); // 多余的
        g_pointerWheelRevoker = g_coreWindow.PointerWheelChanged(winrt::auto_revoke, &OnPointerWheelChanged);
    } catch (const winrt::hresult_error &ex) {
        Logger::ErrorBox(L"订阅 CoreWindowEvents 失败: {}", (const wchar_t *)ex.message().c_str());
    }
}

void UnsubscribeCoreWindowEvents() {
    g_coreWindow = nullptr;
}

void SubscribeAcceleratorEvents() {
    if (!g_coreDispatcher) return;
    try {
        g_acceleratorRevoker = g_coreDispatcher.AcceleratorKeyActivated(winrt::auto_revoke, &OnAcceleratorKeyActivated);
    } catch (const winrt::hresult_error &ex) {
        Logger::ErrorBox(L"订阅 AcceleratorKeyActivated 失败: {}", (const wchar_t *)ex.message().c_str());
    }
}

void UnsubscribeAcceleratorEvents() {
    g_coreDispatcher = nullptr;
}

// Helper Functions
void CreateRenderTarget(IDXGISwapChain *pSwapChain) {
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = desc.BufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
    };
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
    UINT            SwapChainFlags
) {
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
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)desc.BufferDesc.Width, (float)desc.BufferDesc.Height);

    CreateRenderTarget(pSwapChain);

    return hr;
}

static INT64 g_Time = 0;
static INT64 g_TicksPerSecond = 0;

ClientInstance *clientInstance = nullptr;

HRESULT __stdcall hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags) {
    // 初始化阶段
    if (!GuiOverlay::sInitialized) {
        // SetThreadDescription(GetCurrentThread(), L"hkPresent12 Thread!!!!"); // 方便调试

        DXGI_SWAP_CHAIN_DESC desc;
        pSwapChain->GetDesc(&desc);
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        desc.OutputWindow = moduleInfo::gMainWindow = FindChildWindowByClass(moduleInfo::gMainWindow, L"Windows.UI.Core.CoreWindow");

        if (FAILED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_pd3dCommandAllocator))))
            return oPresent12(pSwapChain, SyncInterval, Flags);

        if (FAILED(g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_pd3dCommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList))))
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

        try {
            g_coreWindow = CoreWindow::GetForCurrentThread();
        } catch (const winrt::hresult_error &ex) {
            Logger::ErrorBox(L"获取 CoreWindow 失败: {} ({:X})", ex.message().c_str(), (int32_t)ex.code());
        }
        if (g_coreWindow) {
            // Logger::Info("成功获取 CoreWindow 对象！");
            SubscribeCoreWindowEvents();
            g_coreDispatcher = g_coreWindow.Dispatcher();
            SubscribeAcceleratorEvents();
        } else {
            Logger::WarnBox(L"CoreWindow::GetForCurrentThread() 返回 nullptr。可能不在 UI 线程?");
            // todo: 回退方案
        }

        GuiOverlay::initImGui(moduleInfo::gMainWindow, g_pd3dDevice, g_pd3dSrvDescHeap, APP_NUM_BACK_BUFFERS);

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)desc.BufferDesc.Width, (float)desc.BufferDesc.Height);
        if (!QueryPerformanceFrequency((LARGE_INTEGER *)&g_TicksPerSecond))
            g_TicksPerSecond = 1000; // Fallback if QPF fails
        if (!QueryPerformanceCounter((LARGE_INTEGER *)&g_Time))
            g_Time = GetTickCount64(); // Fallback if QPC fails

        CreateRenderTarget(pSwapChain);

        GuiOverlay::sInitialized = true;
    }

    ImGuiIO &io = ImGui::GetIO();
    POINT    mousePos;
    if (GetCursorPos(&mousePos)) {
        ScreenToClient(moduleInfo::gMainWindow, &mousePos);
        io.AddMousePosEvent((float)mousePos.x, (float)mousePos.y);
    }
    INT64 currentTime;
    if (!QueryPerformanceCounter((LARGE_INTEGER *)&currentTime))
        currentTime = GetTickCount64(); // Use fallback if QPC fails
    io.DeltaTime = (float)(currentTime - g_Time) / g_TicksPerSecond;
    g_Time = currentTime;
    // Ensure DeltaTime is positive and reasonable
    if (io.DeltaTime <= 0.0f)
        io.DeltaTime = 0.00001f;

    GuiOverlay::handleKeyPress();

    ImGui_ImplDX12_NewFrame();
    // ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 绘制窗口
    GuiOverlay::drawGUI();

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

    g_wantCaptureMouse = io.WantCaptureMouse;
    g_wantCaptureKeyboard = io.WantCaptureKeyboard;

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    g_pd3dCommandList->ResourceBarrier(1, &barrier);
    g_pd3dCommandList->Close();

    g_commandQueue->ExecuteCommandLists(1, (ID3D12CommandList *const *)&g_pd3dCommandList);

    return oPresent12(pSwapChain, SyncInterval, Flags);
}

bool InstallDX12Hook() {
    winrt::init_apartment(winrt::apartment_type::multi_threaded);
    CanncelMouseEvent::hook();
    return kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success
        && kiero::bind(140, (void **)&oPresent12, hkPresent12) == kiero::Status::Success
        && kiero::bind(54, (void **)&oExecuteCommandLists, hkExecuteCommandLists) == kiero::Status::Success
        && kiero::bind(145, (void **)&oResizeBuffers, hkResizeBuffers) == kiero::Status::Success;
}

void UninstallDX12Hook() {
    UnsubscribeAcceleratorEvents();
    UnsubscribeCoreWindowEvents();

    GuiOverlay::shutdownImGui();

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

    CanncelMouseEvent::unhook();

    winrt::uninit_apartment();
}
