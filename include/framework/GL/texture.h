#pragma once

#include <memory>

#include "framework/gl.h"

class TextureResource
{
private:
    unsigned int _resource = 0;
public:
    TextureResource();
    TextureResource(const TextureResource& other);
    TextureResource& operator=(const TextureResource& other);
    ~TextureResource();

private:
    void Acquire();
    void Release();
public:
    unsigned int Get();
    operator unsigned int() { return Get(); }
    void Free() { Release(); };
};

class Texture
{
public:
    Texture();
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    ~Texture();

    unsigned int Get();
    operator unsigned int() { return Get(); }
private:
    std::shared_ptr<TextureResource> _texture;
};
