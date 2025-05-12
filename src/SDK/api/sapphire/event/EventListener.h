#pragma once

#include <cstdint>
#include <vector>

class Listeners {
public:
    virtual ~Listeners() {}
};

template <typename EventType>
    requires(std::is_base_of_v<Event, EventType>)
class EventListeners : public Listeners {
    friend class EventManager;
    std::vector<std::function<void(EventType &)>> mListeners;

    using Iterator = typename std::vector<std::function<void(EventType &)>>::iterator;
    using ConstIterator = typename std::vector<std::function<void(EventType &)>>::const_iterator;

public:
    constexpr EventListeners() {}

    constexpr void addListener(std::function<void(EventType &)> fn) {
        this->mListeners.emplace_back(std::move(fn));
    }

    constexpr size_t size() const noexcept {
        return this->mListeners.size();
    }

    constexpr Iterator begin() noexcept {
        return this->mListeners.begin();
    }

    constexpr Iterator end() noexcept {
        return this->mListeners.end();
    }
};