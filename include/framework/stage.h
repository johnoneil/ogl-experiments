#pragma once

#include <framework/i2d.h>
#include <glm/glm.hpp>

#include <framework/color.h>

class Stage2D : public i2D
{
public:
    Stage2D() = default;
    ~Stage2D() = default;
    Stage2D& operator=(const Stage2D& rhs) = delete;
    Stage2D(const glm::vec2 sz);
    Stage2D(const float width, const float height);
public:
    void setSize(const glm::vec2& size) {
        _sz = size;
        _projection = glm::ortho( 0.0f, _sz.x, _sz.y, 0.0f, -5.0f, 5.0f);
    }
    glm::vec2 getSize() const { return _sz;}
    Color getColor() const { return _bgColor; }
    void setColor(const Color& color) { _bgColor = color; }
    //void setProjectionMatrix(const glm::mat4& p) { _projection = p; }
    glm::mat4 GetProjectionMatrix() const { return _projection; }
private:
    bool InitializeImpl() override;
    bool RenderImpl() override;
    glm::mat4 ModelTransformImpl() const override;
private:
    Color _bgColor;
    glm::vec2 _sz;
    glm::mat4 _projection;
};

Stage2D& GetStage2D();