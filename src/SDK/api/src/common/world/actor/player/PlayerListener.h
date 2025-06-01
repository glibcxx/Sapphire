#pragma once

class Player;

// size: 8
class PlayerListener {
public:
    // vtb+0
    virtual ~PlayerListener() = 0;

    // vtb+1
    virtual void onWillChangeDimension(Player &player) = 0;

    // vtb+2
    virtual void onDimensionChanged(Player &player) = 0;

    // vtb+3
    virtual void onPlayerDestruction(Player &player) = 0;
};