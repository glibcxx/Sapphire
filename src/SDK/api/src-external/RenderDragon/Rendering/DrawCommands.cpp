#include "DrawCommands.h"
#include "SDK/core/ApiManager.h"

void dragon::rendering::drawcommands::applyState(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    const cg::math::Rect<uint16_t>                      &scissorRect,
    const dragon::rendering::GPUState                   &state
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x41\x80\x78\x00\x00\x48\x8B\xF9"_sig, // 1.21.50
        &applyState>;
    return (Hook::origin)(commands, scissorRect, state);
}

bool dragon::rendering::drawcommands::bindMesh(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    const DFC::MeshFilter                               &meshFilter
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x8D\x56\x00\x48\x8D\x8D"_sig, // 1.21.50
        &bindMesh>;
    return (Hook::origin)(commands, meshFilter);
}

bool dragon::rendering::drawcommands::extractUniforms(
    dragon::rendering::drawcommands::GraphicsCommandContext &commands,
    dragon::materials::definition::ShaderPipeline            pipeline,
    const DFC::MaterialFilter                               &materialFilter,
    const dragon::materials::MaterialUniformOverrides       *uniformOverrides
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x49\x8B\x06\x48\x8B\x08"_sig, // 1.21.50
        &extractUniforms>;
    return (Hook::origin)(commands, pipeline, materialFilter, uniformOverrides);
}

void dragon::rendering::drawcommands::bindUniforms(
    dragon::rendering::drawcommands::GraphicsCommandContext &commands
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x8B\xC4\x53\x56\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x39"_sig, // 1.21.50
        &bindUniforms>;
    return (Hook::origin)(commands);
}

bool dragon::rendering::drawcommands::submit(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    bool                                                 preserveState,
    bgfx::ViewId                                         id,
    const DFC::Transform                                &transform,
    const dragon::materials::Pass                       &pass,
    int                                                  depth
) {
    using Fn = bool (*)(dragon::rendering::drawcommands::DrawCommandContext &, bool, bgfx::ViewId, const DFC::Transform &, const dragon::materials::Pass &, int);
    using Hook = sapphire::ApiLoader<
        "\x48\x83\xEC\x00\x49\x0F\xBE\x41\x00\x4C\x8B\xD9"_sig, // 1.21.50
        (Fn)&submit>;
    return (Hook::origin)(commands, preserveState, id, transform, pass, depth);
}

bool dragon::rendering::drawcommands::submit(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    bool                                                 preserveState,
    bgfx::ViewId                                         id,
    const glm::tmat4x4<float>                           &transform,
    const dragon::materials::Pass                       &pass,
    int                                                  depth
) {
    using Fn = bool (*)(dragon::rendering::drawcommands::DrawCommandContext &, bool, bgfx::ViewId, const glm::tmat4x4<float> &, const dragon::materials::Pass &, int);
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF9\x45\x0F\xB7\xF0"_sig, // 1.21.50
        (Fn)&submit>;
    return (Hook::origin)(commands, preserveState, id, transform, pass, depth);
}

bool dragon::rendering::drawcommands::submit(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    bool                                                 preserveState,
    bgfx::ViewId                                         id,
    const glm::tmat4x4<float>                           &transform,
    const dragon::materials::PassProgramHandle          &program,
    int                                                  depth
) {
    using Fn = bool (*)(dragon::rendering::drawcommands::DrawCommandContext &, bool, bgfx::ViewId, const glm::tmat4x4<float> &, const dragon::materials::PassProgramHandle &, int);
    using Hook = sapphire::ApiLoader<
        "\x40\x55\x56\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\xB4\x24"_sig, // 1.21.50
        (Fn)&submit>;
    return (Hook::origin)(commands, preserveState, id, transform, program, depth);
}

bool dragon::rendering::drawcommands::submitInstanced(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    bool                                                 preserveState,
    bgfx::ViewId                                         id,
    size_t                                               instanceCount,
    const dragon::materials::PassProgramHandle          &program,
    int                                                  depth
) {
    using Fn = bool (*)(dragon::rendering::drawcommands::DrawCommandContext &, bool, bgfx::ViewId, size_t, const dragon::materials::PassProgramHandle &, int);
    using Hook = sapphire::ApiLoader<
        "\x4C\x8B\xDC\x55\x57\x41\x54\x41\x56\x41\x57"_sig, // 1.21.50
        (Fn)&submitInstanced>;
    return (Hook::origin)(commands, preserveState, id, instanceCount, program, depth);
}

bool dragon::rendering::drawcommands::submitInstanced(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    bool                                                 preserveState,
    bgfx::ViewId                                         id,
    gsl::span<const glm::tmat4x4<float>>                 transforms,
    const dragon::materials::Pass                       &pass,
    int                                                  depth
) {
    using Fn = bool (*)(dragon::rendering::drawcommands::DrawCommandContext &, bool, bgfx::ViewId, gsl::span<const glm::tmat4x4<float>>, const dragon::materials::Pass &, int);
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x44\x89\x74\x24\x00\x48\x89\x54\x24"_sig, // 1.21.50
        (Fn)&submitInstanced>;
    return (Hook::origin)(commands, preserveState, id, transforms, pass, depth);
}

bool dragon::rendering::drawcommands::submitInstanced(
    dragon::rendering::drawcommands::DrawCommandContext &commands,
    bool                                                 preserveState,
    bgfx::ViewId                                         id,
    gsl::span<const glm::tmat4x4<float>>                 transforms,
    const dragon::materials::PassProgramHandle          &program,
    const dragon::materials::PassProgramHandle          &fallbackProgram,
    int                                                  depth
) {
    using Fn = bool (*)(dragon::rendering::drawcommands::DrawCommandContext &, bool, bgfx::ViewId, gsl::span<const glm::tmat4x4<float>>, const dragon::materials::PassProgramHandle &, const dragon::materials::PassProgramHandle &, int);
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x80\xBB\x00\x00\x00\x00\x00\x74"_sig, // 1.21.50
        (Fn)&submitInstanced>;
    return (Hook::origin)(commands, preserveState, id, transforms, program, fallbackProgram, depth);
}