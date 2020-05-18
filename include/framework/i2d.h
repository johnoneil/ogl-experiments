#pragma once
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class i2D : public std::enable_shared_from_this<i2D> 
{
public:
    virtual ~i2D() {};
public:
    void addChild(std::shared_ptr<i2D> child) {
        // TODO: make sure the child is not anywhere in the tree already
        if(auto spt = _parent.lock()) {
            spt->removeChild(child);
        }
        _children.push_back(child);
        child->setParent(shared_from_this());
    };
    void removeChild(std::shared_ptr<i2D>& child) {
    }
    void setParent(std::weak_ptr<i2D> parent) { _parent = parent;}
    std::weak_ptr<i2D> getParent() const { return _parent; }
public:
    bool Initialize() {
        // initialize self then children
        bool success = InitializeImpl();
        #if 0
        if(!success) return success;
        #endif
        for(auto c : _children ) {
            success &= c->Initialize();
        }
        return success;
    }
    bool Render() {
        // Render self, then render children
        bool success = RenderImpl();
        #if 0
        if(!success) return success;
        #endif
        for(auto c : _children ) {
            success &= c->Render();
        }
        return success;
    }
    glm::mat4 GetModelTransform() const {
        glm::mat4 m = ModelTransformImpl();
        glm::mat4 p = glm::mat4(1.0);
        if(auto parent = _parent.lock()) {
            p = parent->GetModelTransform();
        }
        return p * m;
    }
private:
    virtual bool InitializeImpl() = 0;
    virtual bool RenderImpl() = 0;
    virtual glm::mat4 ModelTransformImpl() const = 0;
protected:
    std::vector<std::shared_ptr<i2D>> _children;
    std::weak_ptr<i2D> _parent;
};