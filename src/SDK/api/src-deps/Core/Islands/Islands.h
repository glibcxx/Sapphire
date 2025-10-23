#pragma once

#include "SDK/api/src-deps/Core/Signal/SignalReceiver.h"

namespace Bedrock {

    class ActivationArguments;

    using IslandId = uint16_t;

    // size: 8
    class IIslandCore {
        // vtb+0
        virtual ~IIslandCore();

        // vtb+1
        virtual IslandId getId() = 0;

        // vtb+2
        virtual bool start() = 0;

        // vtb+3
        virtual bool suspend() = 0;

        // vtb+4
        virtual bool resume() = 0;

        // vtb+5
        virtual bool stop() = 0;

        // vtb+6
        virtual void mainUpdate() = 0;

        // vtb+7
        virtual void processActivationArguments(const ActivationArguments &args) = 0;
    };

    // size: 16
    class AppIsland : public IIslandCore {
    public:
        std::unique_ptr<SignalReceiver> mSignalRcvr; // off+8

        // vtb+0
        virtual ~AppIsland() override;

        // vtb+1
        virtual IslandId getId() override;

        // vtb+2
        virtual bool start() override;

        // vtb+3
        virtual bool suspend() override;

        // vtb+4
        virtual bool resume() override;

        // vtb+5
        virtual bool stop() override;

        // vtb+6
        virtual void mainUpdate() override;

        // vtb+7
        virtual void processActivationArguments(const ActivationArguments &args) override;
    };

    // size: 72
    class IslandRegistrationInfo {
    public:
        IslandId              mId;                  // off+0
        std::string           mName;                // off+8
        std::vector<uint16_t> mRequires;            // off+40
        std::shared_ptr<IIslandCore> (*mFactory)(); // off+64
    };

    enum class IslandTransition : int {
        Start = 0,
        Suspend = 1,
        Resume = 2,
        Stop = 3,
    };

    // size: 8
    class IIslandManagerLogger {
    public:
        // vtb+0
        virtual ~IIslandManagerLogger();

        // vtb+1
        virtual void transitionSignaled(IslandTransition) = 0;

        // vtb+2
        virtual void transitionCompleted() = 0;

        // vtb+3
        virtual void islandUpdate(IslandId, std::chrono::nanoseconds) = 0;

        // vtb+4
        virtual void islandTransition(IslandId, IslandTransition, bool, std::chrono::nanoseconds) = 0;
    };

    // size: 8
    class IIslandManager {
    public:
        // vtb+0
        virtual ~IIslandManager();

        // vtb+1
        virtual bool registerIsland(IslandRegistrationInfo &) = 0;

        // vtb+2
        virtual std::shared_ptr<IIslandCore> getIsland(IslandId) const = 0;

        // vtb+3
        virtual void start() = 0;

        // vtb+4
        virtual void suspend() = 0;

        // vtb+5
        virtual void resume() = 0;

        // vtb+6
        virtual void stop() = 0;

        // vtb+7
        virtual bool isTransitionComplete() const = 0;

        // vtb+8
        virtual void update() = 0;
    };

} // namespace Bedrock