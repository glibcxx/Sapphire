#include "BlockSource.h"

bool BlockSource::setBlock(
    const BlockPos              &pos,
    const Block                 &block,
    int                          updateFlags,
    std::shared_ptr<BlockActor>  blockEntity,
    const ActorBlockSyncMessage *syncMsg
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x45\x8B\xE9"_sig,
        &BlockSource::setBlock>;
    return (this->*Hook::origin)(pos, block, updateFlags, blockEntity, syncMsg);
}