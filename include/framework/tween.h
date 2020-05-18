#pragma once

#include "framework/isubsystem.h"
#include <stdio.h>

#include <memory>
#include <functional>
#include <vector>

#include <glm/glm.hpp>
#include <framework/color.h>

class iTween
{
public:
    virtual ~iTween() {};
public:
    virtual void Start() = 0;
    virtual bool Update(const float dt) = 0;
    virtual bool Cancel() = 0;
    virtual bool isPending() const = 0;
    virtual bool isRunning() const = 0;
    virtual bool isComplete() const = 0;
    virtual bool isCanceled() const = 0;
    virtual float getAlpha() const = 0;
public:
    virtual std::shared_ptr<iTween> Then(std::shared_ptr<iTween>) = 0;
    //virtual std::shared_ptr<iTween> Then(std::function<void(void)>) = 0;
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

#if 0
class TweenSeq
{
public:
    TweenSeq();
    TweenSeq(const TweenSeq& other);
    TweenSeq& operator=TweenSeq(const TweenSeq& other);
    ~TweenSeq();
public:
    TweenSeq& Then(const TweenSeq& other) {
        if(&other != this) {
            // queue the element to the end
            if(_next)
                return _next->Then(other);
            else {
                _next = &other;
                return other;
            }
        }

        // and return the end
        return other;
    }
    TweenSeq& Then(std::shared_ptr<iTween> tween) {

    }
private:
    std::unique_ptr<TweenSeq> _next;
    std::shared_ptr<iTween> _tween;
};
#endif

class Tween : public iTween, public std::enable_shared_from_this<Tween>
{
private:
    enum State {
        PENDING,
        RUNNING,
        COMPLETE,
        CANCELED
    };
    State _state = PENDING;
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
        std::function<bool(float, iTween& tween)>onStart,
        std::function<bool(float, iTween& tween)>onUpdate,
        std::function<bool(float, iTween& tween)>onComplete,
        std::function<bool(float, iTween& tween)>onCancel) {
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
    /*
        Start a tween.
        If tween is in Pending state, moves tween to Running.
        If tween is in Running does nothing.
        If tween is in Complete state, does nothing.
     */
    void Start() override {
        if(_state == State::PENDING) {
            _state = State::RUNNING;
        }
    }
    bool Update(const float dt) override {
        if(_state == State::PENDING)
            return false;
        if(_state == State::CANCELED) {
            if(_onCancel)
                _onCancel(_t, *this);
            return true;
        }
        #if 0
        if(_state == State::COMPLETE) {
            if(_onComplete)
                _onComplete(_t, *this);
            return true;
        }
        #endif
        // State RUNNING
        _t += dt;
        bool complete = false;
        if(_onUpdate)
            complete |= _onUpdate(dt, *this);
        if(complete) {
            if(_onComplete)
                _onComplete(dt, *this);
            if(_next)
                _next->Start();
        }
        return complete;
    }
    bool isPending() const override {
        return _state == State::PENDING;
    }
    bool isRunning() const override {
        return _state == State::RUNNING;
    }
    bool isComplete() const override {
        return _state == State::COMPLETE;
    }
    bool isCanceled() const override {
        return _state == State::CANCELED;
    }
    float getAlpha() const override {
        if(_easing && _duration != 0.0f)
            return _easing(_t / _duration);
        return 1.0f;
    }
    /*
        Cancel a RUNNING tween.
     */
    bool Cancel() override {
        if(_state = State::RUNNING)
            _state = State::CANCELED;
    }

    std::shared_ptr<iTween> Then(std::shared_ptr<iTween> tween) override {
        if(_next) {
            _next->Then(tween);
        } else {
            _next = tween;
        }
        return shared_from_this();
    }
    #if 0
    // This impl fails on emscripten. Not sure why
    // Call a given function after the last tween completes
    std::shared_ptr<iTween> Then(std::function<void(void)> f) override {
        #if 1
        if(_next) {
            _next->Then(f);
        } else {
            //auto _oldComplete = _onComplete;
            _onComplete = [f](float dt, iTween& tween)->bool {
                printf("oncomplete method invoked from Then()\n");
                //if(_oldComplete)
                //    _oldComplete(dt, tween);
                //f();
            };
        }
        #endif
        return shared_from_this();
    }
    #endif
private:
    std::function<bool(float, iTween& tween)> _onStart;
    std::function<bool(float, iTween& tween)> _onUpdate;
    std::function<bool(float, iTween& tween)> _onComplete;
    std::function<bool(float, iTween& tween)> _onCancel;
    float _t = 0;
    float _duration = 0;
    std::function<float(float)> _easing;
    std::shared_ptr<iTween> _next;
};

template<typename T>
std::shared_ptr<iTween> TweenPos(std::shared_ptr<T> obj, const glm::vec2 finalPos, const float duration,
    const TweenSystem::Easing easing) {
    std::weak_ptr<T> weakObj = obj;
	glm::vec2 initialPos = obj->GetPos();
	auto tween = Tween::Create(duration, easing,
        nullptr, // onStart
        [=](float dt, iTween& tween)->bool{ // onUpdate
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
        nullptr, // onComplete
        nullptr); // onCancel
    
        return tween;
    }

template<typename T>
std::shared_ptr<iTween> TweenScale(std::shared_ptr<T> obj, const glm::vec2 finalScale, const float duration,
    const TweenSystem::Easing easing) {
    std::weak_ptr<T> weakObj = obj;
    // TODO: this is wrong. We need to get the initial value when the tween starts, not when it is initialized.
	glm::vec2 initialScale = obj->GetScale();
	auto tween = Tween::Create(duration, easing,
        nullptr, // onStart
        [=](float dt, iTween& tween)->bool{ // onUpdate
            auto obj = weakObj.lock();
            if(obj) {
			    float a = tween.getAlpha();
			    if(a >= 1.0f) {
				    obj->SetScale(finalScale);
				    return true;
			    }else{
                    float b = 1.0f - a;
                    obj->SetScale((a*finalScale) + (b*initialScale));
                    return false;
                }
            }else{
                return true; // object no longer around so this tween MUST be done.
            }
			return false;
		},
        nullptr, // onComplete
        nullptr); // onCancel
    
        return tween;
    }


template<typename T>
std::shared_ptr<iTween> TweenColor(std::shared_ptr<T> obj, const Color& finalColor, const float duration,
    const TweenSystem::Easing easing) {
    std::weak_ptr<T> weakObj = obj;
	Color initialColor = obj->GetColor();
	auto tween = Tween::Create(duration, easing,
        nullptr, // onStart
        [=](float dt, iTween& tween)->bool{ // onUpdate
            auto obj = weakObj.lock();
            if(obj) {
			    float a = tween.getAlpha();
			    if(a >= 1.0f) {
				    obj->SetColor(finalColor);
				    return true;
			    }else{
                    float b = 1.0f - a;
                    obj->SetColor((a*finalColor.Vec4()) + (b*initialColor.Vec4()));
                    return false;
                }
            }else{
                return true; // object no longer around so this tween MUST be done.
            }
			return false;
		},
        nullptr, // onComplete
        nullptr); // onCancel
    
        return tween;
    }

    std::shared_ptr<iTween> DummyTween(const float duration, std::function<void(void)> onComplete = nullptr);
    std::shared_ptr<iTween> Pause(const float duration);
