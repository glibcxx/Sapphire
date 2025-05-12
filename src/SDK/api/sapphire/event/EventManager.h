#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <mutex>
#include <type_traits>

#include "Event.h"
#include "EventListener.h"

using ListenerId = uint64_t;

class EventManager {
public:
    SDK_API static EventManager &getInstance();

    EventManager(const EventManager &) = delete;
    EventManager &operator=(const EventManager &) = delete;

    template <typename EventType, typename Func>
        requires std::is_base_of_v<Event, EventType> && std::is_invocable_v<Func, EventType &>
    void registerListener(Func &&fn) {
        std::type_index eventTypeId(typeid(EventType));

        std::lock_guard<std::mutex> lock(this->mMutex);

        auto found = this->mListeners.find(eventTypeId);
        if (found == this->mListeners.end()) {
            found = this->mListeners.emplace(eventTypeId, std::make_shared<EventListeners<EventType>>()).first;
        }
        static_cast<EventListeners<EventType> &>(*found->second).addListener(std::forward<Func>(fn));
    }

    bool unregisterListener(ListenerId id) { return false; }

    template <typename EventType>
        requires std::is_base_of_v<Event, std::remove_cvref_t<EventType>>
    void dispatchEvent(EventType &&event) {
        std::type_index             eventTypeId(typeid(EventType));
        std::lock_guard<std::mutex> lock(this->mMutex);

        auto found = this->mListeners.find(eventTypeId);
        if (found == this->mListeners.end())
            return;
        EventListeners<EventType> &ls = static_cast<EventListeners<EventType> &>(*found->second);
        for (auto &&l : ls) {
            l(event);
        }
    }

private:
    EventManager() = default;
    ~EventManager() = default;

    std::unordered_map<std::type_index, std::shared_ptr<Listeners>> mListeners;

    std::unordered_map<ListenerId, std::type_index> mListenerIdToTypeIndex;

    std::mutex mMutex;

    static std::atomic<ListenerId> sNextListenerId;
};