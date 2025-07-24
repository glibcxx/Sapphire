#pragma once

#include "macros/Macros.h"

namespace sapphire::event {

    class Event {
    public:
        virtual ~Event() = default;
    };

} // namespace sapphire::event