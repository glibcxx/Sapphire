#pragma once

#include "../Mob.h"

class Player : public Mob {
public:
    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x4C\x8B\xF1\x48\x89\x01\x48\x8D\x54\x24")
    SDK_API static const uintptr_t __vftable[];
};