#pragma once

#include "pch.h" // IWYU pragma: keep

namespace RakNet {

    typedef unsigned char  UniqueIDType;
    typedef unsigned short SystemIndex;
    typedef unsigned char  RPCIndex;

    class RakNetGUID {
    public:
        uint64_t    g;
        SystemIndex systemIndex;
    };

} // namespace RakNet