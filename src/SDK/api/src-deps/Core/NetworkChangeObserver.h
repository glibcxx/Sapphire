#pragma once

#include "pch.h" // IWYU pragma: keep

#include "Utility/Observer.h"

// size: 16
class NetworkChangeObserver : public Core::Observer<NetworkChangeObserver, std::mutex> {
};