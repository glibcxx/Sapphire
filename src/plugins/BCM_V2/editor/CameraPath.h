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
    };

    class CameraPath {
    public:
        void addKeyframe(const Keyframe &keyframe);
        void removeKeyframe(size_t tick);

        std::optional<CameraState> getCameraState(size_t tick, float alpha) const;

        auto begin() { return mKeyframes.begin(); }
        auto end() { return mKeyframes.end(); }
        auto begin() const { return mKeyframes.cbegin(); }
        auto end() const { return mKeyframes.cend(); }
        auto cbegin() const { return mKeyframes.cbegin(); }
        auto cend() const { return mKeyframes.cend(); }

        bool            empty() const { return mKeyframes.empty(); }
        size_t          size() const { return mKeyframes.size(); }
        const Keyframe &front() const { return mKeyframes.front(); }
        const Keyframe &back() const { return mKeyframes.back(); }

        void renderPath(
            const std::optional<size_t> &hoveredTick,
            SelectionMode                hoveredMode,
            const std::optional<size_t> &selectedTick,
            SelectionMode                selectedMode
        );

        void recalculateAllHandles();

    private:
        std::vector<Keyframe> mKeyframes;

    public:
        bool mShowPath = true;
        int  mPathDensity = 1;
    };

} // namespace BCM_V2