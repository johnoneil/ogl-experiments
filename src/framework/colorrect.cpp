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

class ColorRectImpl
{
public:
private:
    glm::vec2 _pos = {0.0f, 0.0f};
    glm::vec2 _sz = {0.0f, 0.0f};
    Color _color;
private:
    // gl impl
    Shader _shader;
    // Trianglestrip vertices
    static constexpr float _vertices [] = { 
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    GLuint VAO = 0;
    GLuint VBO = 0;
public:
    ColorRectImpl() {

    }

    #if 0
    ColorRectImp& operator=(const ColorRect& rhs) {
        _impl = std::make_shared<ColorRectImpl>();
        if(&rhs != this) {
            _impl->SetSize(rhs.GetSize());
            _impl->SetPos(rhs.GetPos());
            _impl->SetColor(rhs.GetColor());
        }
        return *this;
    }
    #endif

    ColorRectImpl(const Color& color) :_color(color){
    }

    ColorRectImpl(const glm::vec2& pos, const glm::vec2& sz, const Color& color) 
        :_pos(pos)
        ,_sz(sz)
        ,_color(color)
    {
    }

    void Initialize() {
        _shader = Shader(sVShaderPath, sFShaderPath);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
	    glGenBuffers(1, &VBO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(ColorRectImpl::_vertices), ColorRectImpl::_vertices, GL_STATIC_DRAW);
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
    }

    void DeInitialize() {
        // TODO: free shader resources ?
        // _shader.free();
    }

    void Render() {
        glDisable(GL_DEPTH_TEST);
        _shader.use();
        glm::mat4 projection = glm::ortho( 0.0f, 1024.0f, 768.0f, 0.0f,-5.0f, 5.0f);
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
        model = glm::scale(model, glm::vec3(_sz.x, _sz.y, 1));
        _shader.setMat4("projection", projection);
        _shader.setMat4("model", model);
        _shader.setVec4("c", _color.Vec4());
        glBindVertexArray(VAO);
	    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    glm::vec2 GetPos() const {
        return _pos;
    }

    glm::vec2 GetSize() const {
        return _sz;
    }

    Color GetColor() const {
        return _color;
    }

    void SetPos(const glm::vec2& pos) {
        _pos = pos;
    }

    void SetSize(const glm::vec2& size) {
        _sz = size;
    }

    void SetColor(const Color& color) {
        _color = color;
    }

};

constexpr float ColorRectImpl::_vertices[];

ColorRect::ColorRect() {
    _impl = std::make_shared<ColorRectImpl>();
}

ColorRect& ColorRect::operator=(const ColorRect& rhs) {
    _impl = std::make_shared<ColorRectImpl>();
    if(&rhs != this) {
        _impl->SetSize(rhs.GetSize());
        _impl->SetPos(rhs.GetPos());
        _impl->SetColor(rhs.GetColor());
    }
    return *this;
}

ColorRect::ColorRect(const Color& color) {
    _impl = std::make_shared<ColorRectImpl>(color);
}

ColorRect::ColorRect(const glm::vec2& pos, const glm::vec2& sz, const Color& color) {
    _impl = std::make_shared<ColorRectImpl>(pos, sz, color);
}

void ColorRect::Initialize() {
    _impl->Initialize();
}

void ColorRect::DeInitialize() {
    _impl->DeInitialize();
}

void ColorRect::Render() {
    _impl->Render();
}

glm::vec2 ColorRect::GetPos() const {
    return _impl->GetPos();
}

glm::vec2 ColorRect::GetSize() const {
    return _impl->GetSize();
}

Color ColorRect::GetColor() const {
    return _impl->GetColor();
}

void ColorRect::SetPos(const glm::vec2& pos) {
    _impl->SetPos(pos);
}

void ColorRect::SetSize(const glm::vec2& size) {
    _impl->SetSize(size);
}

void ColorRect::SetColor(const Color& color) {
    _impl->SetColor(color);
}

