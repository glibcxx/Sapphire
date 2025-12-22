#include "CameraPath.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "../math/Interpolation.h"

namespace BCM_V2 {

    using namespace interpolation;

    void recalculateHandlesFor(std::vector<Keyframe> &keyframes, size_t index) {
        if (index >= keyframes.size()) return;

        auto &kf = keyframes[index];

        if (keyframes.size() < 2) {
            kf.controlPointIn = kf.position;
            kf.controlPointOut = kf.position;
            return;
        }

        if (index == 0) {
            kf.controlPointIn = kf.position;
            if (!kf.controlPointOutIsUserModified) {
                kf.controlPointOut = kf.position + (keyframes[index + 1].position - kf.position) * 0.333f;
            }
        } else if (index == keyframes.size() - 1) {
            if (!kf.controlPointInIsUserModified) {
                kf.controlPointIn = kf.position - (kf.position - keyframes[index - 1].position) * 0.333f;
            }
            kf.controlPointOut = kf.position;
        } else {
            glm::vec3 prev_pos = keyframes[index - 1].position;
            glm::vec3 current_pos = kf.position;
            glm::vec3 next_pos = keyframes[index + 1].position;

            glm::vec3 tangent = glm::normalize(next_pos - prev_pos);
            float     dist_prev = glm::distance(current_pos, prev_pos);
            float     dist_next = glm::distance(current_pos, next_pos);

            if (!kf.controlPointInIsUserModified) {
                kf.controlPointIn = current_pos - tangent * dist_prev * 0.333f;
            }
            if (!kf.controlPointOutIsUserModified) {
                kf.controlPointOut = current_pos + tangent * dist_next * 0.333f;
            }
        }
    }

    void CameraPath::addKeyframe(const Keyframe &keyframe) {
        auto it = std::lower_bound(
            mKeyframes.begin(), mKeyframes.end(), keyframe, [](const Keyframe &a, const Keyframe &b) {
                return a.tick < b.tick;
            }
        );

        size_t index;
        if (it != mKeyframes.end() && it->tick == keyframe.tick) {
            index = std::distance(mKeyframes.begin(), it);
            *it = keyframe;
        } else {
            it = mKeyframes.insert(it, keyframe);
            index = std::distance(mKeyframes.begin(), it);
        }

        recalculateHandlesFor(mKeyframes, index);
        if (index > 0) {
            recalculateHandlesFor(mKeyframes, index - 1);
        }
        if (index < mKeyframes.size() - 1) {
            recalculateHandlesFor(mKeyframes, index + 1);
        }
    }

    void CameraPath::removeKeyframe(size_t tick) {
        auto it = std::remove_if(mKeyframes.begin(), mKeyframes.end(), [tick](const Keyframe &kf) {
            return kf.tick == tick;
        });

        if (it == mKeyframes.end()) return;

        size_t index = std::distance(mKeyframes.begin(), it - 1);
        mKeyframes.erase(it, mKeyframes.end());

        if (mKeyframes.empty()) return;

        if (index < mKeyframes.size()) {
            recalculateHandlesFor(mKeyframes, index);
        }
        if (index > 0) {
            recalculateHandlesFor(mKeyframes, index - 1);
        }
    }

    std::optional<CameraState> CameraPath::getCameraState(size_t tick, float alpha) const {
        if (mKeyframes.size() == 0 || tick > mKeyframes.back().tick || tick < mKeyframes.front().tick) {
            return std::nullopt;
        }

        auto it_next = std::upper_bound(mKeyframes.begin(), mKeyframes.end(), tick, [](size_t val, const Keyframe &kf) {
            return val < kf.tick;
        });

        if (it_next == mKeyframes.begin()) {
            return CameraState{mKeyframes.front().position, mKeyframes.front().orientation};
        }

        if (it_next == mKeyframes.end()) {
            return CameraState{mKeyframes.back().position, mKeyframes.back().orientation};
        }

        auto it_prev = std::prev(it_next);

        const Keyframe &prev = *it_prev;
        const Keyframe &next = *it_next;

        size_t segment_duration = next.tick - prev.tick;
        if (segment_duration == 0) {
            return CameraState{next.position, next.orientation};
        }

        double current_tick_in_segment = (tick - prev.tick) + alpha;
        float  t = static_cast<float>(current_tick_in_segment / segment_duration);
        t = glm::clamp(t, 0.0f, 1.0f);

        glm::vec3 pos;
        glm::quat rot;

        switch (prev.interpToNext) {
        case InterpolationType::Linear: {
            pos = glm::mix(prev.position, next.position, t);
            rot = glm::slerp(prev.orientation, next.orientation, t);
            break;
        }
        case InterpolationType::CatmullRom: {
            const Keyframe &p0 = (it_prev == mKeyframes.begin()) ? prev : *std::prev(it_prev);
            const Keyframe &p1 = prev;
            const Keyframe &p2 = next;
            const Keyframe &p3 = (std::next(it_next) == mKeyframes.end()) ? next : *std::next(it_next);

            pos = catmullRom(p0.position, p1.position, p2.position, p3.position, t);

            glm::quat s1 = squad_intermediate(p0.orientation, p1.orientation, p2.orientation);
            glm::quat s2 = squad_intermediate(p1.orientation, p2.orientation, p3.orientation);
            rot = squad(p1.orientation, p2.orientation, s1, s2, t);
            break;
        }
        case InterpolationType::Bezier: {
            const Keyframe &p0 = (it_prev == mKeyframes.begin()) ? prev : *std::prev(it_prev);
            const Keyframe &p1 = prev;
            const Keyframe &p2 = next;
            const Keyframe &p3 = (std::next(it_next) == mKeyframes.end()) ? next : *std::next(it_next);
            pos = bezier(prev.position, prev.controlPointOut, next.controlPointIn, next.position, t);
            rot = squad(p1.orientation, p2.orientation, squad_intermediate(p0.orientation, p1.orientation, p2.orientation), squad_intermediate(p1.orientation, p2.orientation, p3.orientation), t);
            break;
        }
        }

        return CameraState{pos, rot};
    }

    void CameraPath::renderPath() {
        if (!mShowPath || mKeyframes.empty()) return;

        DrawUtils &du = DrawUtils::getInstance();

        if (mKeyframes.size() >= 2) {
            glm::vec3 last_pos;
            bool      first = true;
            for (size_t t = mKeyframes.front().tick; t < mKeyframes.back().tick; t += mPathDensity) {
                if (auto state = this->getCameraState(t, 0.0f)) {
                    if (!first) {
                        du.drawLine(last_pos, state->position, mce::Colors::YELLOW);
                    }
                    last_pos = state->position;
                    first = false;
                }
            }
            if (auto last_state = this->getCameraState(mKeyframes.back().tick, 0.0f)) {
                if (!first && last_pos != last_state->position) {
                    du.drawLine(last_pos, last_state->position, mce::Colors::YELLOW);
                }
            }
        }

        for (const auto &kf : mKeyframes) {
            bool is_selected = mSelectedKeyframeTick.has_value() && *mSelectedKeyframeTick == kf.tick;
            bool is_hovered = mHoveredKeyframeTick.has_value() && *mHoveredKeyframeTick == kf.tick;

            mce::Color kf_color = is_selected && mSelectedMode == SelectionMode::Keyframe
                                    ? mce::Colors::WHITE
                                    : (is_hovered && mHoveredMode == SelectionMode::Keyframe
                                           ? mce::Colors::YELLOW
                                           : mce::Colors::RED);
            du.drawBox({kf.position - 0.1f, kf.position + 0.1f}, kf_color);

            glm::vec3 backward_vector = kf.orientation * glm::vec3(0.0f, 0.0f, 1.0f);
            du.drawLine(kf.position, kf.position - backward_vector * 2.0f, mce::Colors::CYAN);

            if (kf.interpToNext == InterpolationType::Bezier) {
                mce::Color in_color = is_selected && mSelectedMode == SelectionMode::ControlPointIn
                                        ? mce::Colors::WHITE
                                        : (is_hovered && mHoveredMode == SelectionMode::ControlPointIn
                                               ? mce::Colors::YELLOW
                                               : mce::Colors::GREEN);
                mce::Color out_color = is_selected && mSelectedMode == SelectionMode::ControlPointOut
                                         ? mce::Colors::WHITE
                                         : (is_hovered && mHoveredMode == SelectionMode::ControlPointOut
                                                ? mce::Colors::YELLOW
                                                : mce::Colors::BLUE);

                du.drawBox({kf.controlPointIn - 0.05f, kf.controlPointIn + 0.05f}, in_color);
                du.drawBox({kf.controlPointOut - 0.05f, kf.controlPointOut + 0.05f}, out_color);
                du.drawLine(kf.position, kf.controlPointIn, mce::Colors::GREY);
                du.drawLine(kf.position, kf.controlPointOut, mce::Colors::GREY);
            }
        }
    }

} // namespace BCM_V2