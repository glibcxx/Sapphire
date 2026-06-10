#pragma once

#include "pch.h" // IWYU pragma: keep

namespace sapphire::event {

    class Event {
    public:
        virtual ~Event() = default;
    };

} // namespace sapphire::event