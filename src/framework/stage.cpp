#include <framework/stage.h>
#include <framework/gl.h>

Stage Stage::_instance;

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