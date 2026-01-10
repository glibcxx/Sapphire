#pragma once

namespace mce {

    enum class DynamicTexture : uint8_t {
        BrightnessTexture0 = 0,
        BrightnessTexture1 = 1,
        BrightnessTexture2 = 2,
        BrightnessTexture3 = 3,
        SeasonFoliageTexture = 4,
        WeatherOcclusionTexture0 = 5,
        WeatherOcclusionTexture1 = 6,
        WeatherOcclusionTexture2 = 7,
        WeatherOcclusionTexture3 = 8,
        WeatherSnowOcclusionTexture0 = 9,
        WeatherSnowOcclusionTexture1 = 10,
        WeatherSnowOcclusionTexture2 = 11,
        WeatherSnowOcclusionTexture3 = 12,
        Skin = 13,
        LoadingScreen = 14,
        LoadingScreenFont = 15,
        Invalid = 16,
        JpegCommentWriter = 17,
        _count = 18,
    };

}
