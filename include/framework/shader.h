#pragma once

#if 0
#include <glad/glad.h>
#else
#include <framework/gl.h>
#include <framework/GL/shader.h>
#endif
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    resources::ShaderProgram ID;

    Shader() = default;
    Shader(const Shader& other) = default;
    Shader& operator=(const Shader& rhs) = default;
    ~Shader();

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = std::string(""));
    // activate the shader
    // ------------------------------------------------------------------------
    void use();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value);
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value);
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value);
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value);

    void setVec2(const std::string &name, float x, float y);
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value);

    void setVec3(const std::string &name, float x, float y, float z);
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value);

    void setVec4(const std::string &name, float x, float y, float z, float w);

    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat);

    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat);

    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat);

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type);
};
