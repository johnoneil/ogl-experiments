#pragma once

#include <framework/i2d.h>
#include <glm/glm.hpp>

#include <framework/color.h>

class Stage : public i2D
{
public:
    Stage() = default;
    ~Stage() = default;
    Stage& operator=(const Stage& rhs) = delete;
public:
    void setSize(const glm::vec2& size) {_sz = size;}
    glm::vec2 getSize() const { return _sz;}
    Color getColor() const { return _bgColor; }
    void setColor(const Color& color) { _bgColor = color; }
private:
    bool InitializeImpl() override;
    bool RenderImpl() override;
    glm::mat4 ModelTransformImpl() const override;
private:
    Color _bgColor;
    glm::vec2 _sz;
};

Stage& GetStage();