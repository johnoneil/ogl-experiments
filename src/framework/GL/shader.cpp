#include "framework/GL/Shader.h"

#include "framework/gl.h"


using namespace resources;

#if 0
void ShaderProgramResource::Acquire() {
    // TODO: warn if _resource is nonzero
    _resource = glCreateProgram();
}
void ShaderProgramResource::Release() {
    // TODO: warn if _resource is zero
    glDeleteShader(_resource);
    _resource = 0;
}
#endif

ShaderProgramResource::ShaderProgramResource() {}

ShaderProgramResource::ShaderProgramResource(const ShaderProgramResource& other) {
    if(_resource)
        Release();
    _resource = other._resource;
}

ShaderProgramResource& ShaderProgramResource::operator=(const ShaderProgramResource& other) {
    if(&other != this) {
        if(_resource)
            Release();
        _resource = other._resource;
    }
    return *this;
}

ShaderProgramResource::~ShaderProgramResource() {
    if(_resource) {
        Release();
    }
}

void ShaderProgramResource::Acquire() {
    // TODO: warn if _resource is nonzero
    _resource = glCreateProgram();
}
void ShaderProgramResource::Release() {
    // TODO: warn if _resource is zero
    glDeleteShader(_resource);
    _resource = 0;
}

unsigned int ShaderProgramResource::Get() {
    if(!_resource) {
        Acquire();
    }
    return _resource;
}

ShaderProgram::ShaderProgram() {
    _shader = std::make_shared<ShaderProgramResource>();
}

ShaderProgram::ShaderProgram(const ShaderProgram& other) {
    _shader = other._shader;  
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {
    if(&other != this) {
        _shader = other._shader;
    }
    return *this;
}

ShaderProgram::~ShaderProgram() {}

unsigned int ShaderProgram::Get() {
    if(_shader)
        return _shader->Get();
    return 0;
}
