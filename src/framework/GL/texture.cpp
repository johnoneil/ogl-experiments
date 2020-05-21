#include "framework/GL/texture.h"

TextureResource::TextureResource() {}

TextureResource::TextureResource(const TextureResource& other) {
    if(_id)
        Release();
    _id = other._id;
}

TextureResource& TextureResource::operator=(const TextureResource& other) {
    if(&other != this) {
        if(_id != other._id)
            Release();
        _id = other._id;
    }
    return *this;
}

TextureResource::~TextureResource() {
    Release();
}

GLuint TextureResource::ID() {
    // Lazy initialization
    if(_id == 0) {
        glGenTextures(1, &_id);
    }
    return _id;
}

void TextureResource::Release() {
    if(_id) {
        glDeleteTextures(1, &_id);
        _id = 0;
    }
}
