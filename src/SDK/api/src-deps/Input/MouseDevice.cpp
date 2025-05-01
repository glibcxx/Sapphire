#include "MouseDevice.h"

void MouseDevice::feed(
    MouseAction::ActionType action,
    int                     buttonData,
    short                   x,
    short                   y,
    short                   dx,
    short                   dy,
    uint8_t                 a8
) {
    using Hook = core::ApiLoader<
        "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x68\x00\x48\x89\x70\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x44\x0F\xB7\xBC\x24"_sig,
        &MouseDevice::feed>;
    (this->*Hook::origin)(action, buttonData, x, y, dx, dy, a8);
}
