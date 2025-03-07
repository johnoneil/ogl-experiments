#include "framework/canvaselement.h"

#include "framework/stage.h"


void CanvasElement::addChild(std::shared_ptr<CanvasElement> child, const std::string& name) {
    // TODO: make sure the child is not anywhere in the tree already
    if(name.size()) {
        child->SetName(name);
    }
    GetStage2D().Register(child);

    if(auto spt = _parent.lock()) {
        spt->removeChild(child);
    }
    _children.push_back(child);
    child->setParent(shared_from_this());
}

void CanvasElement::removeChild(std::shared_ptr<CanvasElement>& child) {

}

bool CanvasElement::Initialize() {
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
glm::mat4 CanvasElement::Render(const glm::mat4& parentTransform) {
    // Render self, then render children
    auto model = RenderImpl(parentTransform);
    #if 0
    if(!success) return success;
    #endif
    for(auto c : _children ) {
        c->Render(model);
    }
    return model;
}
#if 0
glm::mat4 CanvasElement::GetModelTransform() const {
    #if 1
    glm::mat4 m = ModelTransformImpl();
    glm::mat4 p = glm::mat4(1.0);
    if(auto parent = _parent.lock()) {
        p = parent->GetModelTransform();
    }
    return p * m;
    #else
    return ModelTransformImpl();
    #endif
}
#endif
