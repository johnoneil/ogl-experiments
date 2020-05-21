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
