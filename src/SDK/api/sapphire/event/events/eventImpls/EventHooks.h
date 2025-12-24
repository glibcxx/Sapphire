#pragma once

namespace sapphire::event::impl {

    class EventHooks {
    public:
        static void init();
        static void uninit();
    };

} // namespace sapphire::event::impl