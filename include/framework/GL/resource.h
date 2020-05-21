#pragma once


template <typename T>
class Resource {
public:
    Resource() {}

    Resource(const Resource& other) {
        if(_resource)
            Free();  
        _resource = other._resource;
    }

    Resource& operator=(const Resource& other) {
        if(&other != this) {
            if(_resource != other._resource)
                Free();
            _resource = other._resource;
        }
        return *this;
    }

    virtual ~Resource() {
        Free();
    }

    T Get() {
        // Lazy initialization
        if(_resource == 0) {
            Acquire();
        }
        return _resource;
    }

    operator unsigned int() { return Get(); }

    void Free() {
        if(_resource) {
            Release();
            _resource = 0;
        }
    }
protected:
    T _resource = 0;

protected:
    virtual void Acquire() = 0;
    virtual void Release() = 0;
};
