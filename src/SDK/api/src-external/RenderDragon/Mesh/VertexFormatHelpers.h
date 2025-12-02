#pragma once

#include "VertexFormat.h"
#include "SDK/api/src-external/bgfx/bgfx.h"

namespace dragon::mesh {

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x80\xBD\x00\x00\x00\x00\x00\x75\x00\xC6\x86") // 1.21.50
    SDK_API std::optional<bgfx::VertexDecl> translateFormat(const dragon::mesh::VertexFormat &format);

} // namespace dragon::mesh