#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "framework/shader.h"
#include "framework/gl.h"
#include "framework/canvasElement.h"
#include "framework/color.h"

//#include <glm/gtc/matrix_transform.hpp>

class Image: public CanvasElement
{
private:
    Shader _shader;
    std::string _imagePath;
    // Trianglestrip vertices
    static constexpr float _vertices [] = {
        // Vertices // texture coords
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    GLuint _VAO = 0;
    GLuint _VBO = 0;
    GLuint _texture = 0;
    unsigned int _initialWidth = 0;
    unsigned int _initialHeight = 0;
public:
    Image();
    Image(const Image& other);
    Image& operator=(const Image& other);
    ~Image();
public:
    Image(const std::string& path);
    Image(const std::string& path, const glm::vec2& sz, const glm::vec2& pos);
    Image(const std::string& path, const glm::vec2& pos);
private:
    bool InitializeImpl() override;
    bool RenderImpl() override;
    glm::mat4 ModelTransformImpl() const override;
};