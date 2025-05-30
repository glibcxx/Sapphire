#pragma once

#include <cstdint>

enum class ShowStoreOfferRedirectType : uint8_t {
    MarketplaceOffer = 0,
    DressingRoomOffer = 1,
    ThirdPartyServerPage = 2,
    Count = 3,
};
