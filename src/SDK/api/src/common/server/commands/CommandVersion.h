#pragma once

// size: 8
class CommandVersion {
public:
    int mFrom; // off+0
    int mTo;   // off+4

    CommandVersion(int from = 0, int to = INT_MAX) :
        mFrom(from), mTo(to) {}
};