#pragma once

#include <memory>

#include "framework/gl.h"


namespace resources
{

class BufferResource
{
private:
    unsigned int _resource = 0;
public:
    BufferResource();
    BufferResource(const BufferResource& other);
    BufferResource& operator=(const BufferResource& other);
    ~BufferResource();

private:
    void Acquire();
    void Release();
public:
    unsigned int Get();
    operator unsigned int() { return Get(); }
    void Free() { Release(); };
};

class Buffer
{
public:
    Buffer();
    Buffer(const Buffer& other);
    Buffer& operator=(const Buffer& other);
    ~Buffer();

    unsigned int Get();
    operator unsigned int() { return Get(); }
private:
    std::shared_ptr<BufferResource> _buffer;
};
}; // namespace resources;

