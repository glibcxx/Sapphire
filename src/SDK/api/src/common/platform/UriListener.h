#pragma once

class ActivationUri;

// size: 8
class UriListener {
public:
    // vtb+0
    virtual ~UriListener();

    // vtb+1
    virtual void onUri(const ActivationUri &) = 0;

    // vtb+2
    virtual void tick() = 0;
};