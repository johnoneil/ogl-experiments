#include "framework/text.h"

#include "framework/stage.h"

#if 1
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#include <glm/gtc/matrix_transform.hpp>

constexpr float Character::vertices[];

Character::Character(const unsigned int texture, const glm::ivec2& size, const glm::ivec2& bearing, const unsigned int advance)
    :TextureID(texture)
    ,Size(size)
    ,Bearing(bearing)
    ,Advance(advance)
{};

Character& Character::operator=(const Character& rhs) {
    if(&rhs != this) {
        TextureID = rhs.TextureID;
        Size = rhs.Size;
        Bearing = rhs.Bearing;
        Advance = rhs.Advance;
        // TODO: abstract and manage these resources better.
        _VBO = rhs._VBO;
        _VAO = rhs._VAO;
    }
    return *this;
}

Character::Character(const Character& other) 
    :TextureID(other.TextureID)
    ,Size(other.Size)
    ,Bearing(other.Bearing)
    ,Advance(other.Advance)
    ,_VBO(other._VBO)
    ,_VAO(other._VAO) {

}

bool Font::Load(const std::string& font, const unsigned int size /* = 24*/) {
    _size = size;
    #if defined(__EMSCRIPTEN__)
    _shader = Shader("assets/webgl.text.vs.glsl", "assets/webgl.text.fs.glsl");
    #else
    _shader = Shader("assets/text.vs", "assets/text.fs");
    #endif
    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, _size);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        #if 0
        GLenum err = GL_NO_ERROR;
        if((err = glGetError()) != GL_NO_ERROR) {
            printf("Gl error %d before teximage2d call.\n", err);
        }
        #endif
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_R8, // GL_R8 (instead of GL_RED) give us webgl compat.
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        #if 0
        printf("Uploaded texture with width: %d height: %d\n", face->glyph->bitmap.width, face->glyph->bitmap.rows);
        if((err = glGetError()) != GL_NO_ERROR) {
            printf("Gl error %d AFTER teximage2d call.\n", err);
        }
        #endif
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        #if 0
        printf("glyph texture id: %u\n", texture);
        #endif
        Character character(
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x));

        #if 1
        glGenVertexArrays(1, &character._VAO);
        glGenBuffers(1, &character._VBO);
        glBindVertexArray(character._VAO);
        glBindBuffer(GL_ARRAY_BUFFER, character._VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(character.vertices), character.vertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        #endif

        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
}

void Font::RenderText(const std::string& text,
    const glm::mat4& modelMatrix,
    const Color& color) {
    
    auto _x = 0;
    auto _y = 0;

    #if 1
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    #endif

    // activate corresponding render state
    _shader.use();
    _shader.setVec4("textColor", color.Vec4());
    _shader.setMat4("projection", GetStage2D().GetProjectionMatrix());
    glActiveTexture(GL_TEXTURE0);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = _x + ch.Bearing.x;
        float ypos = _y + (_size - ch.Bearing.y);
        glm::mat4 model = modelMatrix;
        model = glm::translate(model, glm::vec3(xpos, ypos, 0.0f));
        model = glm::scale(model, glm::vec3(ch.Size.x, ch.Size.y, 1.0f));

        _shader.setVec4("textColor", color.Vec4());
        _shader.setMat4("projection", GetStage2D().GetProjectionMatrix());
        _shader.setMat4("model", model);

        glBindVertexArray(ch._VAO);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        _x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 Font::GetRect(const std::string& str) const {
    glm::vec2 rect(0.0f,0.0f);
    float _x = 0;
    for (std::string::const_iterator c = str.begin(); c != str.end(); ++c) {
        
        #if 0
        Character ch = Characters[*c];
        #else
        auto search = Characters.find(*c);
        Character ch = search->second;
        #endif

        float w = _x + ch.Bearing.x + ch.Size.x;
        float top = _size - ch.Bearing.y;
        float bottom = top + ch.Size.y ;
        float h = bottom;

        rect.x = w;
        if(rect.y < h)
            rect.y = h;

        //float xpos = _x + ch.Bearing.x;
        //float ypos = _y + (_size - ch.Bearing.y);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        _x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    return rect;
}

#if 0
Text::Text() {}
Text& Text::operator=(const Text& rhs) {
    if(&rhs != this) {
        _str = rhs._str;
        _pos = rhs._pos;
        _color = rhs._color;
        _scale = rhs._scale;
        _font = rhs._font;
    }
    return *this;
}
#endif

Text::Text(const std::string& str, const glm::vec2& pos, const Color& color, std::shared_ptr<Font> font) 
    :_font(font)
    ,_str(str) {
        _pos = pos;
        _color = color;
    }

bool Text::InitializeImpl() {
    return true;
}

glm::mat4 Text::RenderImpl(const glm::mat4& parentTransform) {
    
    glm::mat4 model = parentTransform;
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y,0.0f));
    model = glm::translate(model, glm::vec3(_center.x * _sz.x, _center.y * _sz.y, 0.0f));
    glm::mat4 noSize = glm::scale(model, glm::vec3(_scale.x, _scale.y, 1));
    model = glm::scale(noSize, glm::vec3(_sz.x, _sz.y, 1));
    model = glm::rotate(model, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-1.0f * _center.x, -1.0f * _center.y, 0.0f));
    
    noSize = glm::rotate(noSize, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    noSize = glm::translate(noSize, glm::vec3(-1.0f * _center.x * _sz.x, -1.0f * _center.y * _sz.y, 0.0f));


    // TODO: text scaling here, not in the font
    //model = glm::scale(model, glm::vec3(_sz.x, _sz.y, 1));
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
    _font->RenderText(_str, model, _color.Vec4());
    return noSize;
}

#if 0
glm::mat4 Text::ModelTransformImpl() const {
    #if 0
    return glm::mat4(1.0);
    #else
    glm::mat4 p = glm::mat4(1.0);
    if(auto parent = _parent.lock()) {
        p = parent->GetModelTransform();
    }
    glm::mat4 m = glm::mat4(1.0);
    m = glm::translate(m, glm::vec3(_pos.x, _pos.y, 0.0f));
    //m = glm::scale(m, glm::vec3(_sz.x, _sz.y, 1));
    return p * m;
    #endif
}
#endif

glm::vec2 Text::GetRect() const {
    if(_font)
        return _font->GetRect(_str);
    return glm::vec2(0.f, 0.0f);
}

std::shared_ptr<Font> Text::GetFont() const {
    return _font;
}

void Text::SetFont(std::shared_ptr<Font> font) {
    _font = font;
}
