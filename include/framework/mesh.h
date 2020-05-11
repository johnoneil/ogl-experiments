#pragma once

#include <framework/gl.h>
#include <framework/shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#if 0
struct Shader {

};
#endif

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh Data
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture>      _textures;
    GLuint _VAO;

    // constructor
    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<Texture>& textures);

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    GLuint _VBO = 0;
    GLuint _EBO = 0;

    // initializes all the buffer objects/arrays
    void setupMesh();
};
