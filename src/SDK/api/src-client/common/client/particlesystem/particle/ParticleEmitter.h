#pragma once

#include <chrono>
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src/common/world/phys/AABB.h"

class Actor;
class ActorUniqueID;
class WeakEntityRef;
class Dimension;
class ExpressionNode;
class ParticleRenderData;
class ClientFrameUpdateContext;

namespace ParticleSystem {

    class ActorBindInfo;

    class ParticleEmitter {
    public:
        // vtb+0
        virtual ~ParticleEmitter() = 0;

        // vtb+1
        virtual void setActorBindInfo(Actor *actor, const HashedString &locator, const Vec3 &offset) = 0;

        // vtb+2
        virtual void setActorBindInfo(WeakEntityRef actor, const HashedString &locator, const Vec3 &offset) = 0;

        // vtb+3
        virtual void setActorBindInfo(Dimension *dimension, ActorUniqueID actorId, const HashedString &locator, const Vec3 &offset) = 0;

        // vtb+4
        virtual void setEnableUpdate(bool enable) = 0;

        // vtb+5
        virtual void setEnableRender(bool enable) = 0;

        // vtb+6
        virtual void runInitializationScript(const ExpressionNode &initializationScript) = 0;

        // vtb+7
        virtual void onBlockChanged(const BlockPos &blockPosition) = 0;

        // vtb+8
        virtual void expire() = 0;

        // vtb+9
        virtual void emitParticleManually(const Vec3 &particlePosition, const Vec3 &velocityAdd, const float scaleMult) = 0;

        // vtb+10
        virtual bool isValid() const = 0;

        // vtb+11
        virtual bool isManualEmitter() const = 0;

        // vtb+12
        virtual bool expirationRequested() const = 0;

        // vtb+13
        virtual bool hasExpired() const = 0;

        // vtb+14
        virtual const AABB &getAABB() const = 0;

        // vtb+15
        virtual const HashedString &getEffectName() const = 0;

        // vtb+16
        virtual size_t getParticleCount() const = 0;

        // vtb+17
        virtual size_t getEffectEmitterCount() const = 0;

        // vtb+18
        virtual size_t getEffectParticleCount() const = 0;

        // vtb+19
        virtual size_t getTotalEmitterCount() const = 0;

        // vtb+20
        virtual size_t getTotalParticleCount() const = 0;

        // vtb+21
        virtual void tick(const std::chrono::nanoseconds &dtIn, const float a) = 0;

        // vtb+22
        virtual void frameUpdate(ClientFrameUpdateContext &clientFrameUpdateContext) = 0;

        // vtb+23
        virtual void extractForRendering(ParticleRenderData &particleRenderData, float a) = 0;

        // vtb+24
        virtual void setManualParticleEmission(bool manualParticleEmission) = 0;

        // vtb+25
        virtual ParticleSystem::ActorBindInfo getActorBindInfo() const = 0;
    };

    class ParticleEmitterActual : public ParticleEmitter {
    public:
        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x44\x0F\x29\xA0\x00\x00\x00\x00\x44\x0F\x29\xA8\x00\x00\x00\x00\x44\x0F\x29\xB0\x00\x00\x00\x00\x44\x0F\x29\xB8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x0F\x28")
        SDK_API virtual void tick(const std::chrono::nanoseconds &dtIn, const float a) override;
    };

} // namespace ParticleSystem