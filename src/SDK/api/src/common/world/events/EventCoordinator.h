#pragma once

#include <functional>
#include <thread>
#include <gsl/gsl>
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "EventResult.h"

// size: 88
template <typename ListenerType>
class EventCoordinator : public Bedrock::EnableNonOwnerReferences {
public:
    bool registerListener(gsl::not_null<ListenerType *> listener) {
        auto it = std::find(this->mListeners.begin(), this->mListeners.end(), listener);
        if (it != this->mListeners.end())
            return false;
        this->mListeners.emplace_back(listener);
        return true;
    }

    void unregisterListener(gsl::not_null<ListenerType *> listener) {
        this->mListeners.erase(std::find(this->mListeners.begin(), this->mListeners.end(), listener));
    }

    std::vector<gsl::not_null<ListenerType *>>                             mListeners;           // off+24
    std::vector<std::function<EventResult(gsl::not_null<ListenerType *>)>> mEventsToProcess;     // off+48
    std::thread::id                                                        mThreadId;            // off+72
    bool                                                                   mThreadIdInitialized; // off+76
    unsigned int                                                           mThreadCheckIndex;    // off+80
};