#pragma once

#include "SDK/api/src-deps/json/Value.h"

// size: 128
class WebToken {
public:
    std::string mHeader;     // off+0
    Json::Value mHeaderInfo; // off+32
    std::string mData;       // off+48
    Json::Value mDataInfo;   // off+80
    std::string mSignature;  // off+96
};