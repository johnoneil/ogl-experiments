#pragma once

#include "framework/shader.h"
#include "framework/i2d.h"
#include "framework/color.h"
#include <string>
#include <memory>

class FontImpl;

class Font
{
private:
    std::shared_ptr<FontImpl> _impl;

public:
    Font();
    Font& operator=(const Font& rhs);
    ~Font();

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
