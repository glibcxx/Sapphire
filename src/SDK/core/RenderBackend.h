#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <d3d11.h>

namespace sapphire::core {

    class RenderBackend {
    public:
        RenderBackend();
        ~RenderBackend();

    private:
        bool _init();
        void _shutdown();

        static HRESULT WINAPI hkPresent12(IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags);

        static void WINAPI hkExecuteCommandLists(ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists);

        static HRESULT WINAPI hkResizeBuffers(
            IDXGISwapChain *pSwapChain,
            UINT            BufferCount,
            UINT            Width,
            UINT            Height,
            DXGI_FORMAT     NewFormat,
            UINT            SwapChainFlags
        );

        // Helper Functions

        bool CreateRenderTargetResources(IDXGISwapChain *pSwapChain);

        void CleanupRenderTargetResources();

        void WaitForGPU();

        HWND FindChildWindowByClass(HWND parent, const wchar_t *className);

        static constexpr int APP_NUM_BACK_BUFFERS = 3;

        typedef HRESULT(WINAPI *Present12_t)(IDXGISwapChain3 *, UINT, UINT);
        typedef void(WINAPI *ExecuteCommandLists_t)(
            ID3D12CommandQueue       *queue,
            UINT                      NumCommandLists,
            ID3D12CommandList *const *ppCommandLists
        );
        typedef HRESULT(WINAPI *ResizeBuffers_t)(
            IDXGISwapChain *This,
            UINT            BufferCount,
            UINT            Width,
            UINT            Height,
            DXGI_FORMAT     NewFormat,
            UINT            SwapChainFlags
        );

        Present12_t           mPresent12 = nullptr;
        ExecuteCommandLists_t mExecuteCommandLists = nullptr;
        ResizeBuffers_t       mResizeBuffers = nullptr;

        ID3D12Device *mD3dDevice = nullptr;

        ID3D12GraphicsCommandList *mD3dCommandList = nullptr;
        ID3D12CommandAllocator    *mD3dCommandAllocator = nullptr;
        ID3D12CommandQueue        *mD3dCommandQueue = nullptr;

        ID3D12DescriptorHeap *mD3dRtvDescHeap = nullptr;

        ID3D12Resource *mD3dMainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};

        ID3D11Device           *mD3d11Device = nullptr;
        ID3D11DeviceContext    *mD3d11DeviceContext = nullptr;
        ID3D11On12Device       *mD3d11On12Device = nullptr;
        ID3D11Resource         *mD3d11WrappedBackBuffer[APP_NUM_BACK_BUFFERS] = {};
        ID3D11RenderTargetView *mD3d11RenderTargetView[APP_NUM_BACK_BUFFERS] = {};
    };

} // namespace sapphire::core