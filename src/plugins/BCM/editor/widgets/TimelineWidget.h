#pragma once

#include <glm/glm.hpp>

class BCMEditor; // Forward declaration

class TimelineWidget {
public:
    explicit TimelineWidget(BCMEditor &editor);
    void render();

private:
    BCMEditor &mEditor;
    float      mZoom = 5.0f;
    float      mOffset = 0.0f;
};