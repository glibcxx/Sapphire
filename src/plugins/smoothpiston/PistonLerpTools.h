#pragma once

#include "SDK/api/src/common/world/level/block/actor/PistonBlockActor.h"

inline float easeInOut(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

/*
    x < newX, y = 0;
    x >= newX, (newX, 0) -> (1, 1) 的一条直线
*/
constexpr float newStartX(float x, float newX) {
    if (x < newX)
        return 0.0f;
    return (x - newX) / (1.0f - newX);
}

// 由于一次推动动画被分成两个阶段进行插值，咱不得不写这么一大坨
template <typename Fn, typename... Args>
constexpr void twoStageLerp(float &alpha, PistonBlockActor &piston, Fn lerpFn, Args &&...args)
    requires(std::is_invocable_r_v<float, Fn, float, Args...>)
{
    bool   isSecondStage = piston.mLastProgress == 0.5f;
    float &lastProgress = piston.mLastProgress;
    float &progress = piston.mProgress;
    if (float delta = progress - lastProgress) {
        bool  expanding = delta > 0.0;
        float midPoint = lerpFn(0.5f, args...);
        (isSecondStage ? lastProgress : progress) = (expanding ? midPoint : (1.0f - midPoint));
        float globalT = isSecondStage ? (0.5f + alpha * 0.5f) : (alpha * 0.5f);
        float range = progress - lastProgress;
        alpha = std::min(
            isSecondStage
                ? (lerpFn(globalT, args...) - midPoint) / (1.0f - midPoint + std::numeric_limits<float>::epsilon())
                : lerpFn(globalT, args...) / (midPoint + std::numeric_limits<float>::epsilon()),
            1.0f
        );
    }
}