#pragma once

#include <memory>

#include "framework/gl.h"


class AttributesResource
{
private:
    unsigned int _resource = 0;
public:
    AttributesResource();
    AttributesResource(const AttributesResource& other);
    AttributesResource& operator=(const AttributesResource& other);
    ~AttributesResource();

private:
    void Acquire();
    void Release();
public:
    unsigned int Get();
    operator unsigned int() { return Get(); }
    void Free() { Release(); };
};

class Attributes
{
public:
    Attributes();
    Attributes(const Attributes& other);
    Attributes& operator=(const Attributes& other);
    ~Attributes();

    unsigned int Get();
    operator unsigned int() { return Get(); }
private:
    std::shared_ptr<AttributesResource> _Attributes;
};

