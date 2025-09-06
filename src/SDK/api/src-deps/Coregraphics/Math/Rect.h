#pragma once

namespace cg::math {

    // size: 4 * sizeof(T)
    template <typename T>
    struct Rect {
        T mX;
        T mY;
        T mWidth;
        T mHeight;
    };

} // namespace cg::math