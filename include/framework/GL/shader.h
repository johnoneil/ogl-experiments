#pragma once

#include <memory>

#include "framework/gl.h"


namespace resources
{

class ShaderProgramResource
{
private:
    unsigned int _resource = 0;
public:
    ShaderProgramResource();
    ShaderProgramResource(const ShaderProgramResource& other);
    ShaderProgramResource& operator=(const ShaderProgramResource& other);
    ~ShaderProgramResource();

private:
    void Acquire();
    void Release();
public:
    unsigned int Get();
    operator unsigned int() { return Get(); }
    void Free() { Release(); };
};

class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(const ShaderProgram& other);
    ShaderProgram& operator=(const ShaderProgram& other);
    ~ShaderProgram();

    unsigned int Get();
    operator unsigned int() { return Get(); }
private:
    std::shared_ptr<ShaderProgramResource> _shader;
};
}; // namespace resources;

