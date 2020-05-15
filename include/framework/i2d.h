#pragma once
#include <memory>
#include <vector>

class i2D : std::enable_shared_from_this<i2D> 
{
public:
    virtual ~i2D() {};
public:
    void addChild(std::shared_ptr<i2D> child) {
        if(auto spt = _parent.lock()) {
            spt->removeChild(child);
        }
        _children.push_back(child);
        #if 0
        child->setParent(shared_from_this());
        #endif
    };
    void removeChild(std::shared_ptr<i2D>& child) {

    }
    void setParent(std::shared_ptr<i2D> parent) { _parent = parent;}
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
private:
    virtual bool InitializeImpl() = 0;
    virtual bool RenderImpl() = 0;
protected:
    std::vector<std::shared_ptr<i2D>> _children;
    std::weak_ptr<i2D> _parent;
};