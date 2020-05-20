#pragma once

#include "framework/canvaselement.h"
#include "framework/color.h"
#include "framework/shader.h"
#include "framework/gl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ColorRect : public CanvasElement {
private:
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
    glm::mat4 RenderImpl(const glm::mat4& parentTransform) override;
};