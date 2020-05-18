#pragma once

#include <framework/color.h>
#include <glm/glm.hpp>
#include <memory>

#include <framework/shader.h>
#include <framework/gl.h>
#include <framework/i2d.h>

#include <glm/gtc/matrix_transform.hpp>


class ColorRect : public i2D {
private:
    glm::vec2 _pos = {0.0f, 0.0f};
    glm::vec2 _sz = {0.0f, 0.0f};
    Color _color;
    Shader _shader;
    // Trianglestrip vertices
    static constexpr float _vertices [] = { 
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    GLuint _VAO = 0;
    GLuint _VBO = 0;
public:
    ColorRect();
    ColorRect& operator=(const ColorRect& rhs);
    ~ColorRect() = default;

public:
    ColorRect(const Color& color);
    ColorRect(const glm::vec2& pos, const glm::vec2& sz, const Color& color);

public:
    bool InitializeImpl() override;
    //void DeInitialize();
    bool RenderImpl() override;
    glm::mat4 ModelTransformImpl() const override;

public:
    glm::vec2 GetPos() const;
    glm::vec2 GetSize() const;
    Color GetColor() const;
    void SetPos(const glm::vec2& pos);
    void SetSize(const glm::vec2& size);
    void SetColor(const Color& color);

private:

};