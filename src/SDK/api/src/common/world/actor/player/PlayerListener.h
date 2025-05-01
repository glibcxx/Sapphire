#pragma once

class Player;

class PlayerListener {
public:
    virtual ~PlayerListener() = 0;

    virtual void onWillChangeDimension(Player &) = 0;

    virtual void onDimensionChanged(Player &) = 0;

    virtual void onPlayerDestruction(Player &) = 0;
};