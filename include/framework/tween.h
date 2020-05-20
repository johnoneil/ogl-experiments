#pragma once

#include "framework/isubsystem.h"
#include <stdio.h>

#include <memory>
#include <functional>
#include <vector>

#include <glm/glm.hpp>
#include <framework/color.h>

class iTween : public std::enable_shared_from_this<iTween>
{
public:
    enum State {
        PENDING,
        RUNNING,
        COMPLETE,
        CANCELED
    };
    State _state = PENDING;
public:
    virtual ~iTween() {};
public:
    void Start() {
        if(_state == State::PENDING) {
            _state = State::RUNNING;
        }
        StartImpl();
    }

    virtual void StartImpl() = 0;

    bool Update(const float dt) {
        if(_state == State::PENDING)
            return false;
        if(_state == State::CANCELED)
            return true;
        _t += dt;
        bool complete = UpdateImpl(dt);
        if(complete) {
            if(_next)
                _next->Start();
        }
        return complete;
    }

    virtual bool UpdateImpl(const float dt) = 0;

    bool Cancel() {
        if(_state = State::RUNNING)
            _state = State::CANCELED;
    }

    bool isPending() const {
        return _state == State::PENDING;
    }
    bool isRunning() const {
        return _state == State::RUNNING;
    }
    bool isComplete() const {
        return _state == State::COMPLETE;
    }
    bool isCanceled() const {
        return _state == State::CANCELED;
    }
    float getAlpha() const {
        if(_easing && _duration != 0.0f)
            return _easing(_t / _duration);
        return 1.0f;
    }
    void setEasing(std::function<float(float)> easing) { _easing = easing; }
    std::function<float(float)> getEasing() const { return _easing; };

public:
    std::shared_ptr<iTween> Then(std::shared_ptr<iTween> tween) {
        if(_next) {
            _next->Then(tween);
        } else {
            _next = tween;
        }
        return shared_from_this();
    }

protected:
    float _t = 0;
    float _duration = 0;
    std::function<float(float)> _easing;
    std::shared_ptr<iTween> _next;
};

class TweenSystem : public iSubsystem
{
public:
    enum Easing {
        NONE = 0,
        LINEAR = 1,
        QUADRATIC_IN,
        QUADRATIC_OUT,
        QUADRATIC_INOUT,
        CUBIC_IN,
        CUBIC_OUT,
        CUBIC_INOUT,
        QUARTIC_IN,
        QUARTIC_OUT,
        QUARTIC_INOUT,
        QUINTIC_IN,
        QUINTIC_OUT,
        QUINTIC_INOUT,
        SIN_IN,
        SIN_OUT,
        SIN_INOUT,
        CIRCULAR_IN,
        CIRCULAR_OUT,
        CIRCULAR_INOUT,
        EXPONENTIAL_IN,
        EXPONENTIAL_OUT,
        EXPONENTIAL_INOUT,
        ELASTIC_IN,
        ELASTIC_OUT,
        ELASTIC_INOUT,
        BACK_IN,
        BACK_OUT,
        BACK_INOUT,
        BOUNCE_IN,
        BOUNCE_OUT,
        BOUNCE_INOUT
    };
private:
    TweenSystem();
    TweenSystem(const TweenSystem& other) = delete;
    TweenSystem& operator=(const TweenSystem& other) = delete;
    ~TweenSystem();
public:
    bool Startup() override;
    bool Shutdown() override;
    void Update(const float dt) override;
    void Render() override;

public:
    void AddTween(std::shared_ptr<iTween> tween, bool start=false);
    void RemoveTween(std::shared_ptr<iTween> tween);

public:
    static std::function<float(float)> GetEasingFunction(const Easing easing);

public:
    static TweenSystem& Get();
private:
    std::vector<std::shared_ptr<iTween>> _tweens;
    bool _initialized = false;
};

template< typename OBJ_TYPE, typename MEMBER_TYPE>
class PosTween : public iTween
{
public:
    PosTween(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& f, const float duration, const TweenSystem::Easing easing) {
        _final = f;
        _obj = obj;
        _duration = duration;
        _easing = TweenSystem::GetEasingFunction(easing);
    }
public:
    void StartImpl() override {
        auto obj = _obj.lock();
        if(obj) {
            _initial = obj->GetPos();
        }
    }
    bool UpdateImpl(const float dt) override {
        bool complete = false;
        auto obj = _obj.lock();
        if(obj) {
            float a = getAlpha();
            if(a >= 1.0f) {
                obj->SetPos(_final);
                complete = true;
            }else{
                float b = 1.0f - a;
                obj->SetPos((a*_final) + (b*_initial));
            }
        }else{
            complete = true;
        }
        return complete;
    }

private:
    std::weak_ptr<OBJ_TYPE> _obj;
    MEMBER_TYPE _initial;
    MEMBER_TYPE _final;
};

template<typename OBJ_TYPE, typename MEMBER_TYPE >
std::shared_ptr<iTween> TweenPos(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& finalPos, const float duration, const TweenSystem::Easing easing) {
            auto tween = std::make_shared<PosTween<OBJ_TYPE, MEMBER_TYPE>>(obj, finalPos, duration, easing);
            TweenSystem::Get().AddTween(tween);
            return tween;
        }

template< typename OBJ_TYPE, typename MEMBER_TYPE>
class ScaleTween : public iTween
{
public:
    ScaleTween(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& f, const float duration, const TweenSystem::Easing easing) {
        _final = f;
        _obj = obj;
        _duration = duration;
        _easing = TweenSystem::GetEasingFunction(easing);
    }
public:
    void StartImpl() override {
        auto obj = _obj.lock();
        if(obj) {
            _initial = obj->GetScale();
        }
    }
    bool UpdateImpl(const float dt) override {
        bool complete = false;
        auto obj = _obj.lock();
        if(obj) {
            float a = getAlpha();
            if(a >= 1.0f) {
                obj->SetScale(_final);
                complete = true;
            }else{
                float b = 1.0f - a;
                obj->SetScale((a*_final) + (b*_initial));
            }
        }else{
            complete = true;
        }
        return complete;
    }

private:
    std::weak_ptr<OBJ_TYPE> _obj;
    MEMBER_TYPE _initial;
    MEMBER_TYPE _final;
};

template<typename OBJ_TYPE, typename MEMBER_TYPE >
std::shared_ptr<iTween> TweenScale(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& finalPos, const float duration, const TweenSystem::Easing easing) {
            auto tween = std::make_shared<ScaleTween<OBJ_TYPE, MEMBER_TYPE>>(obj, finalPos, duration, easing);
            TweenSystem::Get().AddTween(tween);
            return tween;
        }

template< typename OBJ_TYPE, typename MEMBER_TYPE>
class SizeTween : public iTween
{
public:
    SizeTween(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& f, const float duration, const TweenSystem::Easing easing) {
        _final = f;
        _obj = obj;
        _duration = duration;
        _easing = TweenSystem::GetEasingFunction(easing);
    }
public:
    void StartImpl() override {
        auto obj = _obj.lock();
        if(obj) {
            _initial = obj->GetSize();
        }
    }
    bool UpdateImpl(const float dt) override {
        bool complete = false;
        auto obj = _obj.lock();
        if(obj) {
            float a = getAlpha();
            if(a >= 1.0f) {
                obj->SetSize(_final);
                complete = true;
            }else{
                float b = 1.0f - a;
                obj->SetSize((a*_final) + (b*_initial));
            }
        }else{
            complete = true;
        }
        return complete;
    }

private:
    std::weak_ptr<OBJ_TYPE> _obj;
    MEMBER_TYPE _initial;
    MEMBER_TYPE _final;
};

template<typename OBJ_TYPE, typename MEMBER_TYPE >
std::shared_ptr<iTween> TweenSize(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& finalPos, const float duration, const TweenSystem::Easing easing) {
            auto tween = std::make_shared<SizeTween<OBJ_TYPE, MEMBER_TYPE>>(obj, finalPos, duration, easing);
            TweenSystem::Get().AddTween(tween);
            return tween;
        }

template< typename OBJ_TYPE, typename MEMBER_TYPE>
class ColorTween : public iTween
{
public:
    ColorTween(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& f, const float duration, const TweenSystem::Easing easing) {
        _final = f;
        _obj = obj;
        _duration = duration;
        _easing = TweenSystem::GetEasingFunction(easing);
    }
public:
    void StartImpl() override {
        auto obj = _obj.lock();
        if(obj) {
            _initial = obj->GetColor();
        }
    }
    bool UpdateImpl(const float dt) override {
        bool complete = false;
        auto obj = _obj.lock();
        if(obj) {
            float a = getAlpha();
            if(a >= 1.0f) {
                obj->SetColor(_final);
                complete = true;
            }else{
                float b = 1.0f - a;
                obj->SetColor((_final*a) + (_initial*b));
            }
        }else{
            complete = true;
        }
        return complete;
    }

private:
    std::weak_ptr<OBJ_TYPE> _obj;
    MEMBER_TYPE _initial;
    MEMBER_TYPE _final;
};

template<typename OBJ_TYPE, typename MEMBER_TYPE >
std::shared_ptr<iTween> TweenColor(std::shared_ptr<OBJ_TYPE> obj,
        const MEMBER_TYPE& finalPos, const float duration, const TweenSystem::Easing easing) {
            auto tween = std::make_shared<ColorTween<OBJ_TYPE, MEMBER_TYPE>>(obj, finalPos, duration, easing);
            TweenSystem::Get().AddTween(tween);
            return tween;
        }

