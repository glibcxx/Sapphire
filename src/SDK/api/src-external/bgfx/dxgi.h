#pragma once

#include <dxgi1_6.h>
#include <d3dcommon.h>

namespace bgfx {

    struct Caps;

    // size: 448
    struct Dxgi {
    public:
        using AdapterI = IDXGIAdapter3;
        using FactoryI = IDXGIFactory5;
        using SwapChainI = IDXGISwapChain3;
        using OutputI = IDXGIOutput;

        void *m_dxgiDll;      // off+0
        void *m_dxgiDebugDll; // off+8

        D3D_DRIVER_TYPE   m_driverType;  // off+16
        DXGI_ADAPTER_DESC m_adapterDesc; // off+24
        DXGI_OUTPUT_DESC  m_outputDesc;  // off+328

        FactoryI *m_factory; // off+424
        AdapterI *m_adapter; // off+432
        OutputI  *m_output;  // off+440

        SAPPHIRE_API("1.21.50", "lea", "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\x41\xBD") // 1.21.50
        SDK_API bool init(bgfx::Caps &_caps);
    };

    // size: 72
    struct SwapChainDesc {
        uint32_t         width;       // off+0
        uint32_t         height;      // off+4
        DXGI_FORMAT      format;      // off+8
        bool             stereo;      // off+12
        DXGI_SAMPLE_DESC sampleDesc;  // off+16
        uint32_t         bufferUsage; // off+24
        uint32_t         bufferCount; // off+28
        DXGI_SCALING     scaling;     // off+32
        DXGI_SWAP_EFFECT swapEffect;  // off+36
        DXGI_ALPHA_MODE  alphaMode;   // off+40
        uint32_t         flags;       // off+44
        void            *nwh;         // off+48
        void            *ndt;         // off+56
        bool             windowed;    // off+64
    };

} // namespace bgfx