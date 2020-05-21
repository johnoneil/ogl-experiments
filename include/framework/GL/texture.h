#pragma once

#include <memory>

#include "framework/gl.h"
#include "framework/GL/resource.h"

class TextureResource : public Resource<unsigned int>
{
protected:
    void Acquire() override;
    void Release() override;
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
