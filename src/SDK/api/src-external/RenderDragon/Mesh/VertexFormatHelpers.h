#pragma once

#include "VertexFormat.h"
#include "SDK/api/src-external/bgfx/bgfx.h"

namespace dragon::mesh {

    SDK_API std::optional<bgfx::VertexDecl> translateFormat(const dragon::mesh::VertexFormat &format);

}