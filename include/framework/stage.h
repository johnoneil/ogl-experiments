#pragma once

#include "framework/canvaselement.h"
#include "framework/color.h"
#include <unordered_map>
#include <glm/glm.hpp>

class Stage2D : public CanvasElement
{
public:
    Stage2D() {};
    ~Stage2D() = default;
    Stage2D& operator=(const Stage2D& rhs) = delete;
    Stage2D(const glm::vec2 sz);
    Stage2D(const float width, const float height);
public:
    std::shared_ptr<CanvasElement> GetByName(const std::string& name);
    void Register(std::shared_ptr<CanvasElement> element);
private:
    bool InitializeImpl() override;
    glm::mat4 RenderImpl(const glm::mat4& parentTransform) override;
    std::string GenerateName() const override;
private:
    glm::mat4 _projection;
public:
    glm::mat4 GetProjectionMatrix() const { return _projection; }
private:
    std::unordered_map<std::string, std::shared_ptr<CanvasElement>> _names;
};

Stage2D& GetStage2D();