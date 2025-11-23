#include "BlockOccluder.h"

BlockOccluder *BlockOccluder::ctor(
    BlockTessellatorCache            &cache,
    const Block                      &block,
    const BlockPos                   &blockPos,
    const AABB                       &shape,
    const std::vector<unsigned char> &checkFace,
    bool                              renderingGUI,
    const AirAndSimpleBlockBits      *airAndSimpleBlocks
) {
    using Bind = sapphire::bind::Fn<
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x33\xC0\x4D\x8B\xF1"_sig,
        &BlockOccluder::ctor>;
    return (this->*Bind::origin)(cache, block, blockPos, shape, checkFace, renderingGUI, airAndSimpleBlocks);
}