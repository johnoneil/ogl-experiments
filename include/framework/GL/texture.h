#pragma once

#include <memory>

#include "framework/gl.h"

class TextureResource //: public enable_shared_from_this<Texture>
{
public:
    TextureResource();
    TextureResource(const TextureResource& other);
    TextureResource& operator=(const TextureResource& other);
    ~TextureResource();

//public:
//    static std::shared_ptr<Texture> Create(const std::string& filename);

public:
    GLuint ID();
    // Implicity conversion to a gl name
    operator unsigned int() { return ID(); }
    void Release();
private:
    GLuint _id = 0;
};