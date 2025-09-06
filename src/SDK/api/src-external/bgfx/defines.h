#pragma once

#include <cstdint>

#define BGFX_BUFFER_NONE UINT16_C(0x0000)
#define BGFX_BUFFER_COMPUTE_READ UINT16_C(0x0100)  //!< Buffer will be read by shader.
#define BGFX_BUFFER_COMPUTE_WRITE UINT16_C(0x0200) //!< Buffer will be used for writing.
#define BGFX_BUFFER_DRAW_INDIRECT UINT16_C(0x0400) //!< Buffer will be used for storing draw indirect commands.
#define BGFX_BUFFER_ALLOW_RESIZE UINT16_C(0x0800)  //!< Allow dynamic index/vertex buffer resize during update.
#define BGFX_BUFFER_INDEX32 UINT16_C(0x1000)       //!< Index buffer contains 32-bit indices.
#define BGFX_BUFFER_COMPUTE_READ_WRITE (0 | BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_COMPUTE_WRITE)