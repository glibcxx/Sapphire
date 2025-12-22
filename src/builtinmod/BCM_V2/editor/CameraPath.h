#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>

namespace BCM_V2 {

    enum class SelectionMode {
        None,
        Keyframe,
        ControlPointIn,
        ControlPointOut
    };

    struct CameraState {
        glm::vec3 position;
        glm::quat orientation;
    };

    enum class InterpolationType {
        Linear,
        CatmullRom,
        Bezier,
    };

    struct Keyframe {
        size_t    tick;
        glm::vec3 position;
        glm::quat orientation;

        InterpolationType interpToNext = InterpolationType::Bezier;

        // 贝塞尔曲线的控制点
        glm::vec3 controlPointIn;
        glm::vec3 controlPointOut;
        bool      controlPointInIsUserModified = false;
        bool      controlPointOutIsUserModified = false;

        friend constexpr auto operator<=>(const Keyframe &k, size_t tick) { return k.tick <=> tick; }
        friend constexpr auto operator<=>(size_t tick, const Keyframe &k) { return tick <=> k.tick; }
        friend constexpr auto operator==(const Keyframe &k, size_t tick) { return tick == k.tick; }
        friend constexpr auto operator==(size_t tick, const Keyframe &k) { return tick == k.tick; }
    };

    class CameraPath {
    public:
        void addKeyframe(const Keyframe &keyframe);
        void removeKeyframe(size_t tick);

        std::optional<CameraState> getCameraState(size_t tick, float alpha) const;

        constexpr auto begin() { return mKeyframes.begin(); }
        constexpr auto end() { return mKeyframes.end(); }
        constexpr auto begin() const { return mKeyframes.cbegin(); }
        constexpr auto end() const { return mKeyframes.cend(); }
        constexpr auto cbegin() const { return mKeyframes.cbegin(); }
        constexpr auto cend() const { return mKeyframes.cend(); }

        constexpr bool            empty() const { return mKeyframes.empty(); }
        constexpr size_t          size() const { return mKeyframes.size(); }
        constexpr const Keyframe &front() const { return mKeyframes.front(); }
        constexpr const Keyframe &back() const { return mKeyframes.back(); }

        void renderPath();

        auto findKeyframe(size_t tick) {
            return std::find_if(mKeyframes.begin(), mKeyframes.end(), [tick](const Keyframe &kf) {
                return kf.tick == tick;
            });
        }

    private:
        std::vector<Keyframe> mKeyframes;

    public:
        std::optional<size_t> mHoveredKeyframeTick;
        SelectionMode         mHoveredMode = SelectionMode::None;

        std::optional<size_t> mSelectedKeyframeTick;
        SelectionMode         mSelectedMode = SelectionMode::None;

        bool mShowPath = true;
        int  mPathDensity = 1;
    };

} // namespace BCM_V2