#pragma once

#include "pch.h" // IWYU pragma: keep

enum class ShowStoreOfferRedirectType : uint8_t {
    MarketplaceOffer = 0,
    DressingRoomOffer = 1,
    ThirdPartyServerPage = 2,
    Count = 3,
};
