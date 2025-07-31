#include "CameraPath.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "../math/Interpolation.h"
#include "BCMEditor.h"

using namespace interpolation;

void CameraPath::addKeyframe(const Keyframe &keyframe) {
    auto it = std::lower_bound(
        mKeyframes.begin(), mKeyframes.end(), keyframe, [](const Keyframe &a, const Keyframe &b) {
            return a.tick < b.tick;
        }
    );

    if (it != mKeyframes.end() && it->tick == keyframe.tick) {
        *it = keyframe;
    } else {
        mKeyframes.insert(it, keyframe);
    }
    recalculateAllHandles();
}

void CameraPath::removeKeyframe(size_t tick) {
    std::erase_if(mKeyframes, [tick](const Keyframe &kf) {
        return kf.tick == tick;
    });
    recalculateAllHandles();
}

std::optional<CameraState> CameraPath::getCameraState(size_t tick, float alpha) const {
    if (mKeyframes.size() < 2) {
        if (mKeyframes.size() == 1 && tick >= mKeyframes[0].tick) {
            return CameraState{mKeyframes[0].position, mKeyframes[0].orientation};
        }
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

void CameraPath::renderPath(
    const std::optional<size_t> &hoveredTick,
    SelectionMode                hoveredMode,
    const std::optional<size_t> &selectedTick,
    SelectionMode                selectedMode
) {
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
        bool is_selected = selectedTick.has_value() && *selectedTick == kf.tick;
        bool is_hovered = hoveredTick.has_value() && *hoveredTick == kf.tick;

        mce::Color kf_color = is_selected && selectedMode == SelectionMode::Keyframe
                                ? mce::Colors::WHITE
                                : (is_hovered && hoveredMode == SelectionMode::Keyframe
                                       ? mce::Colors::YELLOW
                                       : mce::Colors::RED);
        du.drawBox({kf.position - 0.1f, kf.position + 0.1f}, kf_color);

        glm::vec3 backward_vector = kf.orientation * glm::vec3(0.0f, 0.0f, 1.0f);
        du.drawLine(kf.position, kf.position - backward_vector * 2.0f, mce::Colors::CYAN);

        if (kf.interpToNext == InterpolationType::Bezier) {
            mce::Color in_color = is_selected && selectedMode == SelectionMode::ControlPointIn
                                    ? mce::Colors::WHITE
                                    : (is_hovered && hoveredMode == SelectionMode::ControlPointIn
                                           ? mce::Colors::YELLOW
                                           : mce::Colors::GREEN);
            mce::Color out_color = is_selected && selectedMode == SelectionMode::ControlPointOut
                                     ? mce::Colors::WHITE
                                     : (is_hovered && hoveredMode == SelectionMode::ControlPointOut
                                            ? mce::Colors::YELLOW
                                            : mce::Colors::BLUE);

            du.drawBox({kf.controlPointIn - 0.05f, kf.controlPointIn + 0.05f}, in_color);
            du.drawBox({kf.controlPointOut - 0.05f, kf.controlPointOut + 0.05f}, out_color);
            du.drawLine(kf.position, kf.controlPointIn, mce::Colors::GREY);
            du.drawLine(kf.position, kf.controlPointOut, mce::Colors::GREY);
        }
    }
}

void CameraPath::recalculateAllHandles() {
    if (mKeyframes.size() < 2) {
        if (mKeyframes.size() == 1) {
            mKeyframes[0].controlPointIn = mKeyframes[0].position;
            mKeyframes[0].controlPointOut = mKeyframes[0].position;
        }
        return;
    }

    for (size_t i = 0; i < mKeyframes.size(); ++i) {
        if (i == 0) { 
            mKeyframes[i].controlPointIn = mKeyframes[i].position;
            mKeyframes[i].controlPointOut = mKeyframes[i].position
                                          + (mKeyframes[i + 1].position - mKeyframes[i].position) * 0.333f;
        } else if (i == mKeyframes.size() - 1) {
            mKeyframes[i].controlPointIn = mKeyframes[i].position
                                         - (mKeyframes[i].position - mKeyframes[i - 1].position) * 0.333f;
            mKeyframes[i].controlPointOut = mKeyframes[i].position;
        } else {
            glm::vec3 prev_pos = mKeyframes[i - 1].position;
            glm::vec3 current_pos = mKeyframes[i].position;
            glm::vec3 next_pos = mKeyframes[i + 1].position;

            glm::vec3 tangent = glm::normalize(next_pos - prev_pos);
            float     dist_prev = glm::distance(current_pos, prev_pos);
            float     dist_next = glm::distance(current_pos, next_pos);

            mKeyframes[i].controlPointIn = current_pos - tangent * dist_prev * 0.333f;
            mKeyframes[i].controlPointOut = current_pos + tangent * dist_next * 0.333f;
        }
    }
}
