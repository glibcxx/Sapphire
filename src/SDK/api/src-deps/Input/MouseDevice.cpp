#include "MouseDevice.h"

void MouseDevice::feed(
    char  actionButtonId,
    char  buttonData,
    short x,
    short y,
    short dx,
    short dy,
    bool  forceMotionlessPointer
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x68\x00\x48\x89\x70\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x44\x0F\xB7\xBC\x24"_sig,
        &MouseDevice::feed>;
    (this->*Hook::origin)(actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
}
