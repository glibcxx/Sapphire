#pragma once

#include <d3d11.h>

namespace bgfx {

    // size: ?
    struct NvPhysicalGpuHandle {};

    // size: ?
    struct NvAftermathContextHandle {};

    // size: 48
    struct NvApi {
    public:
        void                           *m_nvApiDll;        // off+0
        bgfx::NvPhysicalGpuHandle      *m_nvGpu;           // off+8
        void                           *m_nvAftermathDll;  // off+16
        bgfx::NvAftermathContextHandle *m_aftermathHandle; // off+24

        void (*nvApiD3D11MultiDrawInstancedIndirect)(ID3D11DeviceContext *, uint32_t, ID3D11Buffer *, uint32_t, uint32_t);        // off+32
        void (*nvApiD3D11MultiDrawIndexedInstancedIndirect)(ID3D11DeviceContext *, uint32_t, ID3D11Buffer *, uint32_t, uint32_t); // off+40
    };

} // namespace bgfx