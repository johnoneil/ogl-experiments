#pragma once
#include <memory>
#include <vector>
#include <string>

#include "framework/color.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CanvasElement : public std::enable_shared_from_this<CanvasElement> 
{
public:
    CanvasElement() {};
    CanvasElement(const CanvasElement& other) = default;
    CanvasElement& operator=(const CanvasElement& other) = default;
    virtual ~CanvasElement() = default;

public:
    void addChild(std::shared_ptr<CanvasElement> child, const std::string& name = std::string(""));
    void removeChild(std::shared_ptr<CanvasElement>& child);
    void setParent(std::weak_ptr<CanvasElement> parent) { _parent = parent;}
    std::weak_ptr<CanvasElement> getParent() const { return _parent; }
public:
    bool Initialize();
    glm::mat4 Render(const glm::mat4& parentTransform = glm::mat4(1.0f));
private:
    // NVI pattern
    virtual bool InitializeImpl() = 0;
    virtual glm::mat4 RenderImpl(const glm::mat4& parentTransform) = 0;
protected:
    std::vector<std::shared_ptr<CanvasElement>> _children;
    std::weak_ptr<CanvasElement> _parent;
protected:
    glm::vec2 _pos = glm::vec2(0.0f, 0.0f);
    glm::vec2 _sz = glm::vec2(1.0f, 1.0f);
    glm::vec2 _center = glm::vec2(0.0f, 0.0f);
    glm::vec2 _scale = glm::vec2(1.0f, 1.0f);
    Color _color = Color::White;
    // Rotation is tracked (2d) via a rotation around z axis (RHR) in radians.
    // It's up to clients to wrap this value around to 0 if they want or not.
    float _rotation = 0.0f;
    #if 0
    glm::vec2 _rotation;
    glm::vec2 _rotationCenter;
    #endif
    std::string _name;
public:
    glm::vec2 GetPos() const { return _pos; }
    glm::vec2 GetSize() const { return _sz; }
    glm::vec2 GetCenter() const { return _center; }
    glm::vec2 GetScale() const { return _scale; }
    Color GetColor() const { return _color; }
    float GetRotation() const { return _rotation; }
    void SetPos(const glm::vec2& pos) { _pos = pos; }
    void SetSize(const glm::vec2& size) { _sz = size; }
    void SetCenter(const glm::vec2& center) { _center = center; }
    void SetScale(const glm::vec2& scale) { _scale = scale; }
    void SetColor(const Color& color) { _color = color; }
    void SetRotation(const float rotation) { _rotation = rotation; }

    std::string GetName() const { return _name; }
    void SetName(const std::string& name) { _name = name; }
};