#include <framework/stage.h>
#include <framework/gl.h>

Stage2D::Stage2D(const glm::vec2 sz)
    :_sz(sz)
    ,_projection(glm::ortho( 0.0f, sz.x, sz.y, 0.0f, -5.0f, 5.0f)){}

Stage2D::Stage2D(const float width, const float height) 
    : _sz(glm::vec2(width, height))
    ,_projection(glm::ortho( 0.0f, width, height, 0.0f, -5.0f, 5.0f)) {}

bool Stage2D::InitializeImpl() {
    // nothing to init
    return true;
}

bool Stage2D::RenderImpl() {
    // clear to bg color
    glClearColor(_bgColor.R(), _bgColor.G(), _bgColor.B(), 1.0f);
    // TODO: probably clear stencil buffer to prevent overwrites
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    return true;
}

glm::mat4 Stage2D::ModelTransformImpl() const {
    return glm::mat4(1.0);
}

Stage2D& GetStage2D() {
    static std::shared_ptr<Stage2D> _stage;
    if(!_stage) {
        _stage = std::make_shared<Stage2D>();
    }
    return *_stage;
}
