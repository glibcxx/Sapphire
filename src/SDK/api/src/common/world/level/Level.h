#pragma once

#include "ILevel.h"
#include "SDK/api/src/common/world/level/BlockSourceListener.h"
#include "src/SDK/api/src/common/world/level/IWorldRegistriesProvider.h"

class Level : public ILevel,
              public BlockSourceListener,
              public IWorldRegistriesProvider {
public:
};