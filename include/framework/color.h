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
    // Helpers for basic math
    Color operator*(const float v);
    Color operator+(const Color& other);

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
    static Color Black;
    static Color Navy;
    static Color Blue;
    static Color Green;
    static Color Teal;
    static Color Lime;
    static Color Aqua;
    static Color Maroon;
    static Color Purple;
    static Color Olive;
    static Color Gray;
    static Color Silver;
    static Color Red;
    static Color Fuchsia;
    static Color Yellow;
    static Color White;
    static Color Orange;
    
    /*
    black 	rgb(0,0,0)	
    navy 	rgb(0,0,128)	
    blue 	rgb(0,0,255)	
    green 	rgb(0,128,0)	
    teal 	rgb(0,128,128)	
    lime 	rgb(0,255,0)	
    aqua 	rgb(0,255,255)	
    maroon 	rgb(128,0,0)	
    purple 	rgb(128,0,128)	
    olive 	rgb(128,128,0)	
    gray 	rgb(128,128,128)	
    silver 	rgb(192,192,192)	
    red 	rgb(255,0,0)	
    fuchsia 	rgb(255,0,255)	
    yellow 	rgb(255,255,0)	
    white 	rgb(255,255,255) */
};