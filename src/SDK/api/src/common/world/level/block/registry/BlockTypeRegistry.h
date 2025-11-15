#pragma once

class Block;
class HashedString;

class BlockTypeRegistry {
public:
    SDK_API static const Block &getDefaultBlockState(const HashedString &name);
};