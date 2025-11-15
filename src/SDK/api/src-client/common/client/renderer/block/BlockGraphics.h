#pragma once

class Block;

class BlockGraphics {
public:
    static const BlockGraphics *getForBlock(const Block &block);
};