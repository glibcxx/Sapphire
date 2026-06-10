#pragma once

#include "pch.h" // IWYU pragma: keep

class ActivationUri {
public:
    std::string                                  mVerb;      // off+0
    std::unordered_map<std::string, std::string> mArguments; // off+32
};