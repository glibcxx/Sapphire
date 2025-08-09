#pragma once

#include <memory>

class IPositionalRandomFactory;

class IRandom {
public:
    // vtb+0
    virtual ~IRandom() = default;

    // vtb+2
    virtual int nextInt() = 0;

    // vtb+1
    virtual int nextInt(int) = 0;

    // vtb+3
    virtual int64_t nextLong() = 0;

    // vtb+4
    virtual bool nextBoolean() = 0;

    // vtb+5
    virtual float nextFloat() = 0;

    // vtb+6
    virtual double nextDouble() = 0;

    // vtb+7
    virtual double nextGaussianDouble() = 0;

    // vtb+8
    virtual void consumeCount(uint32_t) = 0;

    // vtb+9
    virtual std::unique_ptr<IRandom> fork() = 0;

    // vtb+10
    virtual std::unique_ptr<IPositionalRandomFactory> forkPositional();
};