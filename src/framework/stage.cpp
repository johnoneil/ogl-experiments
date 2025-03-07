#include "framework/stage.h"
#include "framework/gl.h"

Stage2D::Stage2D(const glm::vec2 sz)
    :_projection(glm::ortho( 0.0f, sz.x, sz.y, 0.0f, -5.0f, 5.0f)){
        _sz = sz;
    }

Stage2D::Stage2D(const float width, const float height) 
    :_projection(glm::ortho( 0.0f, width, height, 0.0f, -5.0f, 5.0f)) {
        _sz = glm::vec2(width, height);
    }

bool Stage2D::InitializeImpl() {
    _projection = glm::ortho( 0.0f, _sz.x, _sz.y, 0.0f, -5.0f, 5.0f);
    return true;
}

glm::mat4 Stage2D::RenderImpl(const glm::mat4& parentTransform) {
    // clear to bg color
    glClearColor(_color.R(), _color.G(), _color.B(), 1.0f);
    // TODO: probably clear stencil buffer to prevent overwrites
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    return glm::mat4(1.0);
}

static unsigned int nextNameValue = 1;

std::string Stage2D::GenerateName() const {
    unsigned int num = nextNameValue;
    ++nextNameValue;
    return std::string("Stage") + std::to_string(num);
}

std::shared_ptr<CanvasElement> Stage2D::GetByName(const std::string& name) {
    auto e = _names.find(name);
    if(e!=_names.end()) {
        return e->second;
    }
    return nullptr;
}
void Stage2D::Register(std::shared_ptr<CanvasElement> element) {
    auto name = element->GetName();
    if(!name.size()) {
        element->SetName(element->GenerateName());
        name = element->GetName();
    }
    if(name.size()) {
        auto e = _names.find(name);
        if(e!=_names.end()) {
            // TODO: assert or return error.
            return;
        }
        _names[name] = element;
    }
}

Stage2D& GetStage2D() {
    static std::shared_ptr<Stage2D> _stage;
    if(!_stage) {
        _stage = std::make_shared<Stage2D>();
    }
    return *_stage;
}
