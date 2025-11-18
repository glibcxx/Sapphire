#pragma once

class ChestBlockActor;
class ShulkerBoxBlockActor;

// size: 32
class ActorBlockRenderer {
public:
    std::unique_ptr<ChestBlockActor>      mChest;        // off+0
    std::unique_ptr<ChestBlockActor>      mEnderChest;   // off+8
    std::unique_ptr<ShulkerBoxBlockActor> mShulkerBox;   // off+16
    std::unique_ptr<ChestBlockActor>      mTrappedChest; // off+24
};