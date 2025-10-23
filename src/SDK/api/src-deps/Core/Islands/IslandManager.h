#pragma once

#include "Islands.h"

namespace Bedrock {

    enum class IslandState : uint32_t {
        Stopped = 0,
        Suspended = 1,
        Running = 2,
    };

    // size: 144
    class IslandInfo {
    public:
        IslandId                                      mId;           // off+0
        std::vector<IslandId>                         mRequires;     // off+8
        std::vector<IslandId>                         mRequiredBy;   // off+32
        std::function<std::shared_ptr<IIslandCore>()> mFactory;      // off+56
        std::shared_ptr<IIslandCore>                  mIsland;       // off+120
        IslandState                                   mCurrentState; // off+136
        IslandState                                   mTargetState;  // off+140
    };

    // size: 224
    class IslandManager : public IIslandManager, public std::enable_shared_from_this<IslandManager> {
    public:
        IIslandManagerLogger   *mLogger;                    // off+24
        std::mutex              mStateLock;                 // off+32
        bool                    mStateTransitionInProgress; // off+112
        IslandState             mTargetState;               // off+116
        std::mutex              mIslandsLock;               // off+120
        std::vector<IslandInfo> mIslands;                   // off+200

        // vtb+0
        virtual ~IslandManager() override;

        // vtb+0
        virtual bool registerIsland(IslandRegistrationInfo &) override;

        // vtb+0
        virtual std::shared_ptr<IIslandCore> getIsland(IslandId) const override;

        // vtb+0
        virtual void start() override;

        // vtb+0
        virtual void suspend() override;

        // vtb+0
        virtual void resume() override;

        // vtb+0
        virtual void stop() override;

        // vtb+0
        virtual bool isTransitionComplete() const override;

        // vtb+0
        virtual void update() override;
    };

} // namespace Bedrock