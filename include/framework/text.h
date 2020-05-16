#pragma once

#include "framework/shader.h"
#include "framework/i2d.h"
#include "framework/color.h"
#include "framework/gl.h"
#include <glm/glm.hpp>
#include <map>

#include <string>
#include <memory>

class FontImpl;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
class Font
{
private:
    //GLuint _texture = 0;
    Shader _shader;
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::map<GLchar, Character> Characters;

public:
    Font() = default;
    Font& operator=(const Font& rhs) = default;
    ~Font() = default;

public:
    bool Load(const std::string& font);
    void RenderText(const std::string& text,
                    const float x, const float y, const float scale,
                    const glm::vec3& color);

};

class Text : public i2D {
private:
    glm::vec2 _pos = {0.0f, 0.0f};
    //glm::vec2 _sz = {0.0f, 0.0f};
    Color _color;
    std::shared_ptr<Font> _font;
    std::string _str;
    float _scale = 1.0f;
public:
    Text();
    Text& operator=(const Text& rhs);
    ~Text() = default;

public:
    Text(const std::string& str, const glm::vec2& pos, const float scale, const Color& color, std::shared_ptr<Font> font);

public:
    bool InitializeImpl() override;
    //void DeInitialize();
    bool RenderImpl() override;
    glm::mat4 ModelTransformImpl() const override;

public:
    glm::vec2 GetPos() const;
    float GetScale() const;
    Color GetColor() const;
    std::shared_ptr<Font> GetFont() const;
    void SetPos(const glm::vec2& pos);
    void SetScale(const float scale);
    void SetColor(const Color& color);
    void SetFont(std::shared_ptr<Font> font);

private:

};
