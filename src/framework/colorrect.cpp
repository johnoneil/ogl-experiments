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
        _VAO = rhs._VBO;
        //_shader = rhs._shader;
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

glm::mat4 ColorRect::RenderImpl(const glm::mat4& parentTransform) {

    // Translate according to parent's position
    glm::mat4 model = parentTransform;
    model = glm::translate(model, glm::vec3(_pos.x - (_center.x * _sz.x * _scale.x), _pos.y - (_center.y * _sz.y * _scale.y), 0.0f));
    glm::mat4 noSize = glm::scale(model, glm::vec3(_scale.x, _scale.y, 1));
    #if 1
    noSize = glm::rotate(noSize, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));   
    #endif
    model = glm::scale(noSize, glm::vec3(_sz.x, _sz.y, 1));

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

#if 0
glm::mat4 ColorRect::ModelTransformImpl() const {
    // 1) Translate the object to the position, e.g, vec3(position, 0.0f).
    // 2) And then rotate the object.
    // 3) Translate the point back to the origin, e.g, vec3(-originx * scale, -originy * scale, 0.0f).
    // 4) Finally, scale the object.
    glm::mat4 m = glm::mat4(1.0);
    #if 0
    m = glm::translate(m, glm::vec3(_pos.x, _pos.y, 0.0f));
    //m = glm::scale(m, glm::vec3(_sz.x, _sz.y, 1));
    return m;
    #else
    m = glm::translate(m, glm::vec3(_pos.x - (_center.x * _sz.x * _scale.x), _pos.y - (_center.y * _sz.y * _scale.y), 0.0f));

    if(auto parent = _parent.lock()) {
        p = parent->GetModelTransform();
    }

    m = glm::scale(m, glm::vec3(_sz.x * _scale.x, _sz.y * _scale.y, 1));

    return m;
    #endif
}
#endif
