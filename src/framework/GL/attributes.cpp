#include "framework/GL/Attributes.h"

AttributesResource::AttributesResource() {}

AttributesResource::AttributesResource(const AttributesResource& other) {
    if(_resource)
        Release();
    _resource = other._resource;
}

AttributesResource& AttributesResource::operator=(const AttributesResource& other) {
    if(&other != this) {
        if(_resource)
            Release();
        _resource = other._resource;
    }
    return *this;
}

AttributesResource::~AttributesResource() {
    if(_resource) {
        Release();
    }
}

void AttributesResource::Acquire() {
    // TODO: warn if resource is nonzero
    glGenVertexArrays(1, &_resource);

}
void AttributesResource::Release() {
    // TODO: warn if _resource is zero
    glDeleteVertexArrays(1, &_resource);
    _resource = 0;
}

unsigned int AttributesResource::Get() {
    if(!_resource) {
        Acquire();
    }
    return _resource;
}

Attributes::Attributes() {
    _Attributes = std::make_shared<AttributesResource>();
}

Attributes::Attributes(const Attributes& other) {
    _Attributes = other._Attributes;  
}

Attributes& Attributes::operator=(const Attributes& other) {
    if(&other != this) {
        _Attributes = other._Attributes;
    }
    return *this;
}

Attributes::~Attributes() {}

unsigned int Attributes::Get() {
    if(_Attributes)
        return _Attributes->Get();
    return 0;
}
