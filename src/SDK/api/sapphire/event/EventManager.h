#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <mutex>
#include <type_traits>

#include "Event.h"

namespace sapphire::event {

    using ListenerId = uint64_t;

    template <typename EventType>
    class AutoListener {
        ListenerId mId;

    public:
        AutoListener(ListenerId id = 0) :
            mId(id) {}
        ~AutoListener() {
            this->remove();
        }

        AutoListener(AutoListener &&rhs) {
            this->remove();
            this->mId = rhs.mId;
            rhs.mId = 0;
        }

        AutoListener &operator=(AutoListener &&rhs) {
            this->remove();
            this->mId = rhs.mId;
            rhs.mId = 0;
            return *this;
        }

    private:
        void remove();
    };

    class ListenerList {
    public:
        virtual ~ListenerList() {}
    };

    template <typename EventType>
    class EventListenerList : public ListenerList {
    public:
        friend class EventManager;

        struct ListeningFuntion {
            std::function<void(EventType &)> mFn;
            ListenerId                       mId;
        };

        constexpr EventListenerList() {}

        constexpr void addListener(std::function<void(EventType &)> fn, ListenerId id) {
            this->mListeners.emplace_back(std::move(fn), id);
        }

        constexpr bool removeListener(ListenerId id) {
            return std::erase_if(this->mListeners, [id](auto &&l) { return l.mId == id; });
        }

        constexpr size_t size() const noexcept {
            return this->mListeners.size();
        }

        constexpr auto begin() noexcept {
            return this->mListeners.begin();
        }

        constexpr auto end() noexcept {
            return this->mListeners.end();
        }

    private:
        std::vector<ListeningFuntion> mListeners;
    };

    class EventManager {
    public:
        SPHR_API static EventManager &getInstance();

        EventManager(const EventManager &) = delete;
        EventManager &operator=(const EventManager &) = delete;

        template <typename EventType, typename Func>
            requires std::is_base_of_v<Event, EventType> && std::is_invocable_v<Func, EventType &>
        [[nodiscard("Listener will be removed in dtor!")]]
        AutoListener<EventType> registerAutoListener(Func &&fn) {
            std::type_index eventTypeId(typeid(EventType));

            std::lock_guard<std::mutex> lock(this->mMutex);

            auto found = this->mListeners.find(eventTypeId);
            if (found == this->mListeners.end()) {
                found = this->mListeners.emplace(eventTypeId, std::make_shared<EventListenerList<EventType>>()).first;
            }
            ListenerId newId = ++sNextListenerId;
            static_cast<EventListenerList<EventType> &>(*found->second).addListener(std::forward<Func>(fn), newId);
            return AutoListener<EventType>{newId};
        }

        template <typename EventType, typename Func>
            requires std::is_base_of_v<Event, EventType> && std::is_invocable_v<Func, EventType &>
        ListenerId registerListener(Func &&fn) {
            std::type_index eventTypeId(typeid(EventType));

            std::lock_guard<std::mutex> lock(this->mMutex);

            auto found = this->mListeners.find(eventTypeId);
            if (found == this->mListeners.end()) {
                found = this->mListeners.emplace(eventTypeId, std::make_shared<EventListenerList<EventType>>()).first;
            }
            ListenerId newId = ++sNextListenerId;
            static_cast<EventListenerList<EventType> &>(*found->second).addListener(std::forward<Func>(fn), newId);
            return newId;
        }

        template <typename EventType>
        bool unregisterListener(ListenerId id) {
            auto found = this->mListeners.find(typeid(EventType));
            if (found == this->mListeners.end())
                return false;
            else
                return static_cast<EventListenerList<EventType> &>(*found->second).removeListener(id);
        }

        template <typename EventType>
            requires std::is_base_of_v<Event, std::remove_cvref_t<EventType>>
        void dispatchEvent(EventType &&event) {
            std::type_index             eventTypeId(typeid(EventType));
            std::lock_guard<std::mutex> lock(this->mMutex);
            auto                        found = this->mListeners.find(eventTypeId);
            if (found == this->mListeners.end()) return;
            EventListenerList<EventType> &ls = static_cast<EventListenerList<EventType> &>(*found->second);
            for (auto &&l : ls) {
                l.mFn(event);
            }
        }

    private:
        EventManager() = default;
        ~EventManager() = default;

        std::unordered_map<std::type_index, std::shared_ptr<ListenerList>> mListeners;

        std::mutex mMutex;

        SPHR_API static ListenerId sNextListenerId;
    };

    template <class EventType>
    void AutoListener<EventType>::remove() {
        if (this->mId)
            EventManager::getInstance().unregisterListener<EventType>(this->mId);
    }

} // namespace sapphire::event