#pragma once

class iSubsystem
{
public:
virtual ~iSubsystem(){};
public:
    virtual bool Startup() = 0;
    virtual bool Shutdown() = 0;
    virtual void Update(const float dt) = 0;
    virtual void Render() = 0;
};