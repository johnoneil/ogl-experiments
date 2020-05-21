#include "framework/GL/texture.h"

void TextureResource::Acquire() {
    // TODO: warn if _resource is nonzero
    glGenTextures(1, &_resource);
}
void TextureResource::Release() {
    // TODO: warn if _resource is zero
    glDeleteTextures(1, &_resource);
    _resource = 0;
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
