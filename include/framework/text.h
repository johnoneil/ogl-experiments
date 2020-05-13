#pragma once

#include "framework/shader.h"
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
