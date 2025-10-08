#pragma once

// size: 8
class PrintStream {
public:
    // off+0
    virtual ~PrintStream() = default;

    // off+1
    virtual void print(const std::string &s) {}
};