#include "VertexFormatHelpers.h"

std::optional<bgfx::VertexDecl> dragon::mesh::translateFormat(const dragon::mesh::VertexFormat &format) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x80\xBD\x00\x00\x00\x00\x00\x75\x00\xC6\x86"_sig, // 1.21.50
        translateFormat>;
    return (Hook::origin)(format);
}