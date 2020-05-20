#include <framework/image.h>

#include <framework/shader.h>
#include <framework/gl.h>
#include <framework/stage.h>

#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#if !defined(__EMSCRIPTEN__)
static const std::string sVShaderPath("assets/image.vs.glsl");
static const std::string sFShaderPath("assets/image.fs.glsl");
#else
static const std::string sVShaderPath("assets/webgl.image.vs.glsl");
static const std::string sFShaderPath("assets/webgl.image.fs.glsl");
#endif

constexpr float Image::_vertices[];

Image::Image() {}

Image::Image(const Image& other) {

}

Image::~Image() {

}

Image& Image::operator=(const Image& rhs) {
    if(&rhs != this) {
        _sz = rhs._sz;
        _pos = rhs._pos;
        _color = rhs._color;
        _imagePath = rhs._imagePath;
        _VBO = rhs._VBO;
        _VAO = rhs._VBO;
        //_shader = rhs._shader;
    }
    return *this;
}

Image::Image(const std::string& path)
    :_imagePath(path){
}
Image::Image(const std::string& path, const glm::vec2& pos, const glm::vec2& sz)
    :_imagePath(path) {
        _sz = sz;
        _pos = pos;
}
Image::Image(const std::string& path, const glm::vec2& pos)
    :_imagePath(path) {
        // take initial size (since not specified) from image dimensions in pixels.
        _sz = glm::vec2(0.0f, 0.0f);
        _pos = pos;
}

bool Image::InitializeImpl() {
    _shader = Shader(sVShaderPath, sFShaderPath);
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Image::_vertices), Image::_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,  // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        4*sizeof(float),    // stride
        (void*)0    // array buffer offset
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,  // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        4*sizeof(float),    // stride
        (void*)(2*sizeof(float))    // array buffer offset
    );
    glBindVertexArray(0);

    auto w = 0, h = 0, c = 0;
    #if 0
    stbi_set_flip_vertically_on_load(true);
    #endif
    auto image = stbi_load(_imagePath.c_str(),
                                    &w,
                                    &h,
                                    &c,
                                    STBI_rgb_alpha);
    if(image) {
        _initialWidth = w;
        _initialHeight = h;
        if(_sz == glm::vec2(0.0f, 0.0f))
            _sz = glm::vec2(_initialWidth, _initialHeight);

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(image);
    }
    return true;
}

#if 0
void Image::DeInitialize() {
}
#endif

bool Image::RenderImpl() {

    // Translate according to parent's position
    glm::mat4 model = GetModelTransform();
    
    glDisable(GL_DEPTH_TEST);
    _shader.use();
    _shader.setMat4("projection", GetStage2D().GetProjectionMatrix());
    _shader.setMat4("model", model);
    _shader.setVec4("c", _color.Vec4());
    glBindVertexArray(_VAO);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    return true;
}

glm::mat4 Image::ModelTransformImpl() const {
    #if 1
    // 1) Translate the object to the position, e.g, vec3(position, 0.0f).
    // 2) And then rotate the object.
    // 3) Translate the point back to the origin, e.g, vec3(-originx * scale, -originy * scale, 0.0f).
    // 4) Finally, scale the object.
    glm::mat4 m = glm::mat4(1.0);
    m = glm::translate(m, glm::vec3(_pos.x - (_center.x * _sz.x * _scale.x), _pos.y - (_center.y * _sz.y * _scale.y), 0.0f));
    m = glm::scale(m, glm::vec3(_sz.x * _scale.x, _sz.y * _scale.y, 1));
    return m;
    #else
    glm::mat4 p = glm::mat4(1.0);
    if(auto parent = _parent.lock()) {
        p = parent->GetModelTransform();
    }
    glm::mat4 m = glm::mat4(1.0);
    m = glm::translate(m, glm::vec3(_pos.x, _pos.y, 0.0f));
    //m = glm::scale(m, glm::vec3(_sz.x, _sz.y, 1));
    m = p * m;
    m = glm::scale(m, glm::vec3(_sz.x * _scale.x, _sz.y * _scale.y, 1));
    return m;
    #endif
}


