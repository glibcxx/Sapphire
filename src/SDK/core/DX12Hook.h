#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class DX12Hook {
public:
    static bool install();
    static void uninstall();

private:
    static HRESULT __stdcall hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags);

    static void __stdcall hkExecuteCommandLists(ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists);

    static HRESULT __stdcall hkResizeBuffers(
        IDXGISwapChain *pSwapChain,
        UINT            BufferCount,
        UINT            Width,
        UINT            Height,
        DXGI_FORMAT     NewFormat,
        UINT            SwapChainFlags
    );

    // Helper Functions

    static void CreateRenderTarget(IDXGISwapChain *pSwapChain);

    static void CleanupRenderTarget(IDXGISwapChain *pSwapChain);

    static void WaitForGPU();

    static HWND FindChildWindowByClass(HWND parent, const wchar_t *className);

    static constexpr int APP_NUM_BACK_BUFFERS = 3;

    typedef HRESULT(__stdcall *Present12_t)(IDXGISwapChain3 *, UINT, UINT);
    typedef void(__stdcall *ExecuteCommandLists_t)(
        ID3D12CommandQueue       *queue,
        UINT                      NumCommandLists,
        ID3D12CommandList *const *ppCommandLists
    );
    typedef HRESULT(__stdcall *ResizeBuffers_t)(
        IDXGISwapChain *This,
        UINT            BufferCount,
        UINT            Width,
        UINT            Height,
        DXGI_FORMAT     NewFormat,
        UINT            SwapChainFlags
    );

    inline static Present12_t           oPresent12 = nullptr;
    inline static ExecuteCommandLists_t oExecuteCommandLists = nullptr;
    inline static ResizeBuffers_t       oResizeBuffers = nullptr;

    inline static ID3D12Device *pd3dDevice = nullptr;

    inline static ID3D12GraphicsCommandList *pd3dCommandList = nullptr;
    inline static ID3D12CommandAllocator    *pd3dCommandAllocator = nullptr;
    inline static ID3D12CommandQueue        *pd3dCommandQueue = nullptr;

    inline static ID3D12DescriptorHeap *pd3dRtvDescHeap = nullptr;
    inline static ID3D12DescriptorHeap *pd3dSrvDescHeap = nullptr;

    inline static ID3D12Resource *pd3dMainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};
};