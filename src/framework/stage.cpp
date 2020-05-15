#include <framework/stage.h>
#include <framework/gl.h>

bool Stage::InitializeImpl() {
    // nothing to init
    return true;
}

bool Stage::RenderImpl() {
    // clear to bg color
    glClearColor(_bgColor.R(), _bgColor.G(), _bgColor.B(), 1.0f);
    // TODO: probably clear stencil buffer to prevent overwrites
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    return true;
}

glm::mat4 Stage::ModelTransformImpl() const {
    return glm::mat4(1.0);
}

Stage& GetStage() {
    static std::shared_ptr<Stage> _stage;
    if(!_stage) {
        _stage = std::make_shared<Stage>();
    }
    return *_stage;
}
