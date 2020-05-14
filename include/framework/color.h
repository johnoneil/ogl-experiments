/*
color.h
 */
#pragma once

#include <glm/glm.hpp>

class Color {
public:
    Color();
    Color& operator=(const Color& rhs);
    ~Color() = default;

public:
    Color(const glm::vec4& c);
    Color(const glm::vec3& c);
    Color(const float r, const float g, const float b, const float a = 1.0f);

public:
    float R() const {return _color.x;}
    float G() const {return _color.y;}
    float B() const {return _color.z;}
    float A() const {return _color.w;}

    float& R() { return _color.x;}
    float& G() { return _color.y;}
    float& B() { return _color.z;}
    float& A() { return _color.w;}

    glm::vec4 Vec4() const { return _color; }
    glm::vec3 Vec3() const { return glm::vec3(_color.x, _color.y, _color.z);}

private:
    glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color White;
    static Color Black;
};