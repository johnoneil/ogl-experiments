#include <framework/text.h>

#if 1
#include <ft2build.h>
#include FT_FREETYPE_H
#endif
#include <map>

#include <glm/gtc/matrix_transform.hpp> // glm::orth

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};


class FontImpl {
private:
    //GLuint _texture = 0;
    Shader _shader;
    GLuint VAO = 0;
    GLuint VBO = 0;
    std::map<GLchar, Character> Characters;
public:
    FontImpl() {
    }

    bool Load(const std::string& font) {
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
        FT_Set_Pixel_Sizes(face, 0, 48);

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
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                // TODO: remove cast
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        
        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return true;
    }
    void RenderText(const std::string& text,
                    const float x, const float y, const float scale,
                    const glm::vec3& color) {
        auto _x = x;
        auto _y = y;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // activate corresponding render state	
        _shader.use();
        //glUniform3f(glGetUniformLocation(_shader.ID, "textColor"), color.x, color.y, color.z);
        _shader.setVec3("textColor", color);
        // TODO: access screen dimensions properly
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        _shader.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = Characters[*c];

            float xpos = _x + ch.Bearing.x * scale;
            float ypos = _y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            _x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

Font::Font() {
    _impl = std::make_shared<FontImpl>();
}

Font& Font::operator=(const Font& rhs) {
    if(&rhs != this)
        _impl = rhs._impl;
    return *this;
}

Font::~Font() {

}

bool Font::Load(const std::string& font) {
    return _impl->Load(font);
}

void Font::RenderText(const std::string& text,
    const float x, const float y, const float scale,
    const glm::vec3& color) {
        return _impl->RenderText(text, x, y, scale, color);
}