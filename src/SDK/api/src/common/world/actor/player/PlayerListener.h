#pragma once

class Player;

// size: 8
class PlayerListener {
public:
    // vtb+0
    virtual ~PlayerListener();

    // vtb+1
    virtual void onWillChangeDimension(Player &player);

    // vtb+2
    virtual void onDimensionChanged(Player &player);

    // vtb+3
    virtual void onPlayerDestruction(Player &player);
};