#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "framework/shader.h"
#include "framework/gl.h"
#include "framework/canvaselement.h"
#include "framework/color.h"
#include "framework/GL/texture.h"
#include "framework/GL/buffer.h"
#include "framework/GL/attributes.h"

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
    Attributes _VAO;
    resources::Buffer _VBO;
    Texture _texture;
    unsigned int _initialWidth = 0;
    unsigned int _initialHeight = 0;
public:
    Image();
    Image(const Image& other);
    Image& operator=(const Image& other);
    ~Image();
public:
    Image(const std::string& path);
    Image(const std::string& path, const glm::vec2& pos, const glm::vec2& sz);
    Image(const std::string& path, const glm::vec2& pos);
private:
    bool InitializeImpl() override;
    glm::mat4 RenderImpl(const glm::mat4& parentTransform) override;
    std::string GenerateName() const override;
};