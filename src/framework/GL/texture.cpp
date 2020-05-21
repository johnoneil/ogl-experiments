#include "framework/GL/texture.h"

TextureResource::TextureResource() {}

TextureResource::TextureResource(const TextureResource& other) {
    if(_resource)
        Release();
    _resource = other._resource;
}

TextureResource& TextureResource::operator=(const TextureResource& other) {
    if(&other != this) {
        if(_resource)
            Release();
        _resource = other._resource;
    }
    return *this;
}

TextureResource::~TextureResource() {
    if(_resource) {
        Release();
    }
}

void TextureResource::Acquire() {
    // TODO: warn if _resource is nonzero
    glGenTextures(1, &_resource);
}
void TextureResource::Release() {
    // TODO: warn if _resource is zero
    glDeleteTextures(1, &_resource);
    _resource = 0;
}

unsigned int TextureResource::Get() {
    if(!_resource) {
        Acquire();
    }
    return _resource;
}

Texture::Texture() {
    _texture = std::make_shared<TextureResource>();
}

Texture::Texture(const Texture& other) {
    _texture = other._texture;  
}

Texture& Texture::operator=(const Texture& other) {
    if(&other != this) {
        _texture = other._texture;
    }
    return *this;
}

Texture::~Texture() {}

unsigned int Texture::Get() {
    if(_texture)
        return _texture->Get();
    return 0;
}
