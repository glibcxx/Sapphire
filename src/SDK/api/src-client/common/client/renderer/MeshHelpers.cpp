#include "MeshHelpers.h"

void MeshHelpers::renderMeshImmediately(
    ScreenContext &screenContext, Tessellator &tessellator, const mce::MaterialPtr &material, void *a4
) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xC6\x43\x00\x00\xF3\x0F\x10\x1D\x00\x00\x00\x00\x0F\x57\xD2"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\xC6\x44\x24\x00\x00\xF3\x0F\x10\x1D"_sig,
#endif
        &MeshHelpers::renderMeshImmediately>;
    Hook::origin(screenContext, tessellator, material, a4);
}
