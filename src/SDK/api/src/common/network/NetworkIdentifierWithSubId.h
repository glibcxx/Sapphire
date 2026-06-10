#pragma once

#include "pch.h" // IWYU pragma: keep

#include "NetworkIdentifier.h"
#include "SDK/api/src/common/CommonTypes.h"

struct NetworkIdentifierWithSubId {
    NetworkIdentifier id;
    SubClientId       subClientId;
};
