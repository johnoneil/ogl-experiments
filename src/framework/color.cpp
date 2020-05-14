#include <framework/color.h>

Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
Color Color::Black(1.0f, 1.0f, 1.0f, 1.0f);

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
