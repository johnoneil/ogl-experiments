#pragma once

#include "framework/canvaselement.h"
#include "framework/color.h"
#include <glm/glm.hpp>


class Stage2D : public CanvasElement
{
public:
    Stage2D() {};
    ~Stage2D() = default;
    Stage2D& operator=(const Stage2D& rhs) = delete;
    Stage2D(const glm::vec2 sz);
    Stage2D(const float width, const float height);
private:
    bool InitializeImpl() override;
    bool RenderImpl() override;
    glm::mat4 ModelTransformImpl() const override;
private:
    glm::mat4 _projection;
public:
    glm::mat4 GetProjectionMatrix() const { return _projection; }
};

Stage2D& GetStage2D();