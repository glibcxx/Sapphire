#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src/common/util/Factory.h"

class BiomeRegistry;
class BlockPalette;
class FeatureRegistry;
class FeatureTypeFactory;
class JigsawStructureRegistry;
class StructureManager;
class BiomeComponentFactory;
class SurfaceBuilderRegistry;
class Dimension;
class Level;
class Scheduler;

class IWorldRegistriesProvider {
public:
    // vtb+0
    virtual ~IWorldRegistriesProvider() {}

    // vtb+1
    virtual BiomeRegistry &getBiomeRegistry() = 0;

    // vtb+2
    virtual const BiomeRegistry &getBiomeRegistry() const = 0;

    // vtb+3
    virtual BlockPalette &getBlockPalette() = 0;

    // vtb+4
    virtual const BlockPalette &getBlockPalette() const = 0;

    // vtb+5
    virtual FeatureRegistry &getFeatureRegistry() = 0;

    // vtb+6
    virtual const FeatureRegistry &getFeatureRegistry() const = 0;

    // vtb+7
    virtual FeatureTypeFactory &getFeatureTypeFactory() = 0;

    // vtb+8
    virtual const FeatureTypeFactory &getFeatureTypeFactory() const = 0;

    // vtb+9
    virtual JigsawStructureRegistry &getJigsawStructureRegistry() = 0;

    // vtb+10
    virtual const JigsawStructureRegistry &getJigsawStructureRegistry() const = 0;

#if MC_VERSION >= v1_21_50
    // vtb+11
    virtual class StructureSpawnRegistry &getStructureSpawnRegistry() = 0;

    // vtb+12
    virtual const StructureSpawnRegistry &getStructureSpawnRegistry() const = 0;
#endif

    // 1.21.2: vtb+11, 1.21.50+: vtb+13
    virtual StructureManager &getStructureManager() = 0;

    // 1.21.2: vtb+12, 1.21.50+: vtb+14
    virtual const StructureManager &getStructureManager() const = 0;

    // 1.21.2: vtb+13, 1.21.50+: vtb+15
    virtual BiomeComponentFactory &getBiomeComponentFactory() = 0;

    // 1.21.2: vtb+14, 1.21.50+: vtb+16
    virtual const BiomeComponentFactory &getBiomeComponentFactory() const = 0;

    // 1.21.2: vtb+15, 1.21.50+: vtb+17
    virtual SurfaceBuilderRegistry &getSurfaceBuilderRegistry() = 0;

    // 1.21.2: vtb+16, 1.21.50+: vtb+18
    virtual const SurfaceBuilderRegistry &getSurfaceBuilderRegistry() const = 0;

    // 1.21.2: vtb+17, 1.21.50+: vtb+19
    virtual OwnerPtrFactory<Dimension, Level &, Scheduler &> &getDimensionFactory() = 0;

    // 1.21.2: vtb+18, 1.21.50+: vtb+20
    virtual const OwnerPtrFactory<Dimension, Level &, Scheduler &> &getDimensionFactory() const = 0;

#if MC_VERSION >= v1_21_50
    // vtb+21
    virtual class ActorDefinitionGroup *getEntityDefinitions() const = 0;
#endif
};