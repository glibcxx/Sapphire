#pragma once

#include <mutex>
#include "Utility/Observer.h"

// size: 16
class NetworkChangeObserver : public Core::Observer<NetworkChangeObserver, std::mutex> {
};