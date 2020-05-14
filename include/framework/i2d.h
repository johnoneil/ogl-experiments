#pragma once
#include <memory>
#include <vector>

class i2D : std::enable_shared_from_this<i2D> 
{
public:
    virtual ~i2D() {};
public:
    void addChild(std::shared_ptr<i2D>& child) {
        if(child->getParent()) {
            child->getParent()->removeChild(child);
        }
        _children.push_back(child);
        child->setParent(std::shared_ptr<i2D>(this));
    };
    void removeChild(std::shared_ptr<i2D>& child) {

    }
    void setParent(std::shared_ptr<i2D>&& parent) { _parent = parent;}
    std::shared_ptr<i2D> getParent() const { return _parent; }
public:
    bool Initialize() {
        // initialize self then children
        bool success = InitializeImpl();
        if(!success) return success;
        for(auto c : _children ) {
            success &= c->Initialize();
        }
        return success;
    }
    bool Render() {
        // Render self, then render children
        bool success = RenderImpl();
        if(!success) return success;
        for(auto c : _children ) {
            success &= c->Render();
        }
        return success;
    }
private:
    virtual bool InitializeImpl() = 0;
    virtual bool RenderImpl() = 0;
protected:
    std::vector<std::shared_ptr<i2D>> _children;
    std::shared_ptr<i2D> _parent;
};