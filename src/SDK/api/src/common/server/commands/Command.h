#pragma once

#include "CommandFlag.h"
#include "CommandOrigin.h"
#include "CommandOutput.h"

class CommandRegistry;

// size: 32
class Command {
public:
    int                    mVersion = 0;                                        // off+8
    const CommandRegistry *mRegistry = nullptr;                                 // off+16
    int                    mCommandSymbol = -1;                                 // off+24
    CommandPermissionLevel mPermissionLevel = CommandPermissionLevel::Internal; // off+28
    CommandFlag            mFlags;                                              // off+30

    Command() = default;

    // vtb+0
    virtual ~Command() = default;

    // vtb+1
    virtual bool collectOptionalArguments() { return true; }

    // vtb+2
    virtual void execute(const CommandOrigin &origin, CommandOutput &output) const = 0;
};

// size: 32
class CommandRawText {
public:
    std::string mText; // off+0
};

// size: 16
class CommandPosition {
public:
    Vec3 mOffset;    // off+0
    bool mRelativeX; // off+12
    bool mRelativeY; // off+13
    bool mRelativeZ; // off+14
    bool mLocal;     // off+15
};

// size: 16
class CommandPositionFloat : public CommandPosition {};

// size: 64
class RelativeFloat {
public:
    float mOffset;   // off+0
    bool  mRelative; // off+4
};

// size: 8
class CommandWildcardInt {
public:
    bool mIsWildcard; // off+0
    int  mValue;      // off+4
};

// size: 12
class CommandIntegerRange {
public:
    int  mMinValue;  // off+0
    int  mMaxValue;  // off+4
    bool mInvert;    // off+8
    bool mInclusive; // off+9
};

// size: 32
class CommandFilePath {
public:
    std::string mText; // off+0

    constexpr bool operator==(const CommandFilePath &inPath) const = default;
};