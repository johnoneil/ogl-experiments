#include "framework/canvaselement.h"


void CanvasElement::addChild(std::shared_ptr<CanvasElement> child) {
    // TODO: make sure the child is not anywhere in the tree already
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
bool CanvasElement::Render() {
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
glm::mat4 CanvasElement::GetModelTransform() const {
    return ModelTransformImpl();
}
