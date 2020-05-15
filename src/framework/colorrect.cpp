#include <framework/colorrect.h>

#include <framework/shader.h>
#include <framework/gl.h>

#include <glm/gtc/matrix_transform.hpp>

#if !defined(__EMSCRIPTEN__)
static const std::string sVShaderPath("assets/colorrect.vs.glsl");
static const std::string sFShaderPath("assets/colorrect.fs.glsl");
#else
static const std::string sVShaderPath("assets/webgl.colorrect.vs.glsl");
static const std::string sFShaderPath("assets/webgl.colorrect.fs.glsl");
#endif

constexpr float ColorRect::_vertices[];

ColorRect::ColorRect() {}

ColorRect& ColorRect::operator=(const ColorRect& rhs) {
    if(&rhs != this) {
        _sz = rhs._sz;
        _pos = rhs._pos;
        _color = rhs._color;
        _VBO = rhs._VBO;
        _VAO = rhs._VBO;
        //_shader = rhs._shader;
    }
    return *this;
}

ColorRect::ColorRect(const Color& color)
    :_color(color){

}

ColorRect::ColorRect(const glm::vec2& pos, const glm::vec2& sz, const Color& color)
    :_pos(pos)
    ,_sz(sz)
    ,_color(color) {
}

bool ColorRect::InitializeImpl() {
    _shader = Shader(sVShaderPath, sFShaderPath);
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ColorRect::_vertices), ColorRect::_vertices, GL_STATIC_DRAW);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,  // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        2*sizeof(float),    // stride
        (void*)0    // array buffer offset
    );
    glBindVertexArray(0);
    return true;
}

#if 0
void ColorRect::DeInitialize() {
}
#endif

bool ColorRect::RenderImpl() {
    glDisable(GL_DEPTH_TEST);
    _shader.use();
    glm::mat4 projection = glm::ortho( 0.0f, 1024.0f, 768.0f, 0.0f,-5.0f, 5.0f);
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(_sz.x, _sz.y, 1));
    _shader.setMat4("projection", projection);
    _shader.setMat4("model", model);
    _shader.setVec4("c", _color.Vec4());
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    return true;
}

glm::vec2 ColorRect::GetPos() const {
    return _pos;
}

glm::vec2 ColorRect::GetSize() const {
    return _sz;
}

Color ColorRect::GetColor() const {
    return _color;
}

void ColorRect::SetPos(const glm::vec2& pos) {
    _pos = pos;;
}

void ColorRect::SetSize(const glm::vec2& size) {
    _sz = size;
}

void ColorRect::SetColor(const Color& color) {
    _color = color;
}

