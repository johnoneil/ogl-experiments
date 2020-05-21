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