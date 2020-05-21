#include "framework/GL/buffer.h"

#include "framework/gl.h"


using namespace resources;

#if 0
void BufferResource::Acquire() {
    // TODO: warn if _resource is nonzero
    glGenBuffers(1, &_resource);
}
void BufferResource::Release() {
    // TODO: warn if _resource is zero
    //glDeleteBuffers(1, &_resource);
    _resource = 0;
}
#endif

BufferResource::BufferResource() {}

BufferResource::BufferResource(const BufferResource& other) {
    if(_resource)
        Release();
    _resource = other._resource;
}

BufferResource& BufferResource::operator=(const BufferResource& other) {
    if(&other != this) {
        if(_resource)
            Release();
        _resource = other._resource;
    }
    return *this;
}

BufferResource::~BufferResource() {
    if(_resource) {
        Release();
    }
}

void BufferResource::Acquire() {
    // TODO: warn if _resource is nonzero
    glGenBuffers(1, &_resource);
}
void BufferResource::Release() {
    // TODO: warn if _resource is zero
    //glDeleteBuffers(1, &_resource);
    _resource = 0;
}

unsigned int BufferResource::Get() {
    if(!_resource) {
        Acquire();
    }
    return _resource;
}

Buffer::Buffer() {
    _buffer = std::make_shared<BufferResource>();
}

Buffer::Buffer(const Buffer& other) {
    _buffer = other._buffer;  
}

Buffer& Buffer::operator=(const Buffer& other) {
    if(&other != this) {
        _buffer = other._buffer;
    }
    return *this;
}

Buffer::~Buffer() {}

unsigned int Buffer::Get() {
    if(_buffer)
        return _buffer->Get();
    return 0;
}
