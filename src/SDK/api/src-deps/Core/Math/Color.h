#pragma once

namespace mce {

    class Color {
    public:
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 0.0f;

        constexpr Color() {}
        constexpr Color(float r, float g, float b, float a) :
            r(r), g(g), b(b), a(a) {}

        class VanillaColors;
    };

    namespace Colors {
        constexpr Color WITHE{1.0f, 1.0f, 1.0f, 1.0f};
        constexpr Color GREY{0.5f, 0.5f, 0.5f, 1.0f};
        constexpr Color BLACK{0.0f, 0.0f, 0.0f, 1.0f};
        constexpr Color RED{1.0f, 0.0f, 0.0f, 1.0f};
        constexpr Color GREEN{0.0f, 1.0f, 0.0f, 1.0f};
        constexpr Color BLUE{0.0f, 0.0f, 1.0f, 1.0f};
        constexpr Color YELLOW{1.0f, 1.0f, 0.0f, 1.0f};
        constexpr Color ORANGE{1.0f, 0.5f, 0.0f, 1.0f};
        constexpr Color PURPLE{0.5f, 0.0f, 0.5f, 1.0f};
        constexpr Color CYAN{0.0f, 1.0f, 1.0f, 1.0f};
        constexpr Color PINK{1.0f, 0.0f, 1.0f, 1.0f};
        constexpr Color NIL{0.0f, 0.0f, 0.0f, 0.0f};

    } // namespace Colors

} // namespace mce