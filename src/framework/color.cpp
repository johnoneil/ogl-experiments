#include <framework/color.h>

Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
Color Color::Navy(0.0f, 0.0f, 0.5f, 1.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
Color Color::Green(0.0f, 0.5f, 0.0f, 1.0f);
Color Color::Teal(0.0f, 0.5f, 0.5f, 1.0f);
Color Color::Lime(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::Aqua(0.0f, 1.0f, 1.0f, 1.0f);
Color Color::Maroon(0.5f, 0.0f, 0.0f, 1.0f);
Color Color::Purple(0.5f, 0.0f, 5.0f, 1.0f);
Color Color::Olive(0.5f, 0.5f, 0.0f, 1.0f);
Color Color::Gray(0.5f, 0.5f, 0.5f, 1.0f);
Color Color::Silver(0.75f, 0.75f, 0.75f, 1.0f);
Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::Fuchsia(1.0f, 0.0f, 1.0f, 1.0f);
Color Color::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);

Color::Color() {

}

Color& Color::operator=(const Color& rhs) {
    if(&rhs != this)
        _color = rhs._color;
    return *this;
}

Color::Color(const glm::vec4& c) :_color(c){

}

Color::Color(const glm::vec3& c) :_color(c.x, c.y, c.z, 1.0f){

}

Color::Color(const float r, const float g, const float b, const float a /* = 1.0f*/) 
    :_color(r, g, b, a) {
    }

    // Helpers for basic math
Color Color::operator*(const float v) {
    return Color(v * _color);
}

Color Color::operator+(const Color& other) {
    return Color(_color + other._color);
}
