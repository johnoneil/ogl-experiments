#pragma once

#include "framework/isubsystem.h"

#include <memory>
#include <functional>
#include <vector>

#include <glm/glm.hpp>

class iTween
{
public:
    virtual ~iTween() {};
public:
    virtual bool Update(const float dt) = 0;
    virtual bool Cancel() = 0;
    virtual bool isComplete() const = 0;
    virtual float getAlpha() const = 0;
};

class Tween;

class TweenSystem : public iSubsystem
{
public:
    friend class Tween;
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

    static std::function<float(float)> GetEasingFunction(const Easing easing);

public:
    static TweenSystem& Get();
private:
    std::vector<std::shared_ptr<iTween>> _tweens;
    bool _initialized = false;
};


class Tween : public iTween
{
public:
    Tween() {

    }
    Tween(const Tween& other) {

    }
    Tween& operator=(const Tween& other) {

    }
    ~Tween() {

    }
public:
    static std::shared_ptr<iTween> Create(const float duration, const TweenSystem::Easing easing,
        std::function<bool(float, Tween& tween)>onStart,
        std::function<bool(float, Tween& tween)>onUpdate,
        std::function<bool(float, Tween& tween)>onComplete,
        std::function<bool(float, Tween& tween)>onCancel) {
            auto tween = std::make_shared<Tween>();
            tween->_onStart = onStart;
            tween->_onUpdate = onUpdate;
            tween->_onComplete = onComplete;
            tween->_onCancel = onCancel;

            tween->_t = 0;
            tween->_duration = duration;
            tween->_easing = TweenSystem::GetEasingFunction(easing);

            TweenSystem::Get()._tweens.push_back(tween);
        
            return tween;
        }
    bool Update(const float dt) override {
        _t += dt;
        bool complete = false;
        if(_onUpdate)
            complete |= _onUpdate(dt, *this);
        complete |= isComplete();
        if(complete && _onComplete)
            _onComplete(dt, *this);
        return complete;
    }
    bool isComplete() const override {
        return _t >= _duration;
    }
    float getAlpha() const override {
        if(_easing && _duration != 0.0f)
            return _easing(_t / _duration);
        return 0.0f;
    }
    bool Cancel() override {
        return true;
    }
private:
    std::function<bool(float, Tween& tween)> _onStart;
    std::function<bool(float, Tween& tween)> _onUpdate;
    std::function<bool(float, Tween& tween)> _onComplete;
    std::function<bool(float, Tween& tween)> _onCancel;
    float _t = 0;
    float _duration = 0;
    std::function<float(float)> _easing;
};

template<typename T>
std::shared_ptr<iTween> TweenPos(std::shared_ptr<T> obj, const glm::vec2 finalPos, const float duration,
    const TweenSystem::Easing easing) {
    std::weak_ptr<T> weakObj = obj;
	glm::vec2 initialPos = obj->GetPos();
	auto tween = Tween::Create(duration, easing,
        nullptr, // onStart
        [=](float dt, Tween& tween)->bool{ // onUpdate
            auto obj = weakObj.lock();
            if(obj) {
			    float a = tween.getAlpha();
			    if(a >= 1.0f) {
				    obj->SetPos(finalPos);
				    return true;
			    }else{
                    float b = 1.0f - a;
                    obj->SetPos((a*finalPos) + (b*initialPos));
                    return false;
                }
            }else{
                return true; // object no longer around so this tween MUST be done.
            }
			return false;
		},
        [=](float dt, Tween& tween)->bool { // onComplete
			//printf("Tween complete\n");
			return tween.isComplete();
		},
        nullptr); // onCancel
    
        return tween;
    }
