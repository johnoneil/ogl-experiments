#pragma once

#include <framework/color.h>
#include <glm/glm.hpp>
#include <memory>

class ColorRectImpl;

class ColorRect {
public:
    ColorRect();
    ColorRect& operator=(const ColorRect& rhs);
    ~ColorRect() = default;

public:
    ColorRect(const Color& color);
    ColorRect(const glm::vec2& pos, const glm::vec2& sz, const Color& color);

public:
    void Initialize();
    void DeInitialize();
    void Render();

public:
    glm::vec2 GetPos() const;
    glm::vec2 GetSize() const;
    Color GetColor() const;
    void SetPos(const glm::vec2& pos);
    void SetSize(const glm::vec2& size);
    void SetColor(const Color& color);

private:
    std::shared_ptr<ColorRectImpl> _impl;
};