#pragma once

#include "framework/shader.h"
#include "framework/canvaselement.h"
#include "framework/color.h"
#include "framework/gl.h"
#include "framework/GL/texture.h"
#include "framework/GL/buffer.h"
#include <glm/glm.hpp>
#include <map>

#include <string>
#include <memory>

class FontImpl;

struct Character {
    Texture TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
    constexpr static float vertices[] = {
        // vertex   // texcoord
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    resources::Buffer _VBO;
    GLuint _VAO = 0;
    Character() = default;
    Character(const Character& other);
    Character& operator=(const Character& rhs);
    ~Character() = default;
    Character(Texture& texture, const glm::ivec2& size, const glm::ivec2& bearing, const unsigned int advance);
};
class Font
{
private:
    Shader _shader;
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::map<GLchar, Character> Characters;
    unsigned int _size = 24;

public:
    Font() = default;
    Font& operator=(const Font& rhs) = default;
    ~Font() = default;

public:
    bool Load(const std::string& font, const unsigned int size = 24);
    void RenderText(const std::string& text,
                    const glm::mat4& modelMatrix,
                    const Color& color);
    glm::vec2 GetRect(const std::string& str) const;

    unsigned int GetSize() const { return _size; }

};

class Text : public CanvasElement {
private:
    std::shared_ptr<Font> _font;
    std::string _str;
public:
    Text() = default;
    Text(const Text& other) = default;
    Text& operator=(const Text& rhs) = default;
    ~Text() = default;

public:
    Text(const std::string& str, const glm::vec2& pos, const Color& color, std::shared_ptr<Font> font);

public:
    bool InitializeImpl() override;
    //void DeInitialize();
    glm::mat4 RenderImpl(const glm::mat4& parentTransform) override;

public:
    std::shared_ptr<Font> GetFont() const;
    void SetFont(std::shared_ptr<Font> font);
    glm::vec2 GetRect() const;
};
