#include <framework/colorrect.h>

#include <framework/shader.h>
#include <framework/gl.h>
#include <framework/stage.h>

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
        _VAO = rhs._VAO;
        _shader = rhs._shader;
    }
    return *this;
}

ColorRect::ColorRect(const Color& color) {
    _color = color;
}

ColorRect::ColorRect(const glm::vec2& pos, const glm::vec2& sz, const Color& color) {
        _pos = pos;
        _sz = sz;
        _color = color;
}

bool ColorRect::InitializeImpl() {
    _shader = Shader(sVShaderPath, sFShaderPath);
    //glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    //glGenBuffers(1, &_VBO);
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

glm::mat4 ColorRect::RenderImpl(const glm::mat4& parentTransform) {

    glm::mat4 model = parentTransform;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y,0.0f));
    model = glm::translate(model, glm::vec3(_center.x * _sz.x, _center.y * _sz.y, 0.0f));
    glm::mat4 noSize = glm::scale(model, glm::vec3(_scale.x, _scale.y, 1));
    model = glm::scale(noSize, glm::vec3(_sz.x, _sz.y, 1));
    model = glm::rotate(model, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-1.0f * _center.x, -1.0f * _center.y, 0.0f));
    
    noSize = glm::rotate(noSize, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    noSize = glm::translate(noSize, glm::vec3(-1.0f * _center.x * _sz.x, -1.0f * _center.y * _sz.y, 0.0f));

    glDisable(GL_DEPTH_TEST);
    _shader.use();
    _shader.setMat4("projection", GetStage2D().GetProjectionMatrix());
    _shader.setMat4("model", model);
    _shader.setVec4("c", _color.Vec4());
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    return noSize;
}

static unsigned int nextNameValue = 1;

std::string ColorRect::GenerateName() const {
    unsigned int num = nextNameValue;
    ++nextNameValue;
    return std::string("ColorRect") + std::to_string(num);
}
