#pragma once

// size: 8
class CommandVersion {
public:
    int mFrom; // off+0
    int mTo;   // off+4

    CommandVersion(int from = 1, int to = std::numeric_limits<int>::max()) :
        mFrom(from), mTo(to) {}
};