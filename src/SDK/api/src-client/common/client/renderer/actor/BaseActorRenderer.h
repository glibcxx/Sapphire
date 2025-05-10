#pragma once

#include <cstdint>
#include "ActorShaderManager.h"

class BaseActorRenderer : public ActorShaderManager {
public:
    // vtb+0
    virtual ~BaseActorRenderer() = 0;
};