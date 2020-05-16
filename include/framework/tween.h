#pragma once

#include "framework/isubsystem.h"

#include <memory>
#include <functional>
#include <vector>
//#include "tweeny.h"

class iTween
{
public:
    virtual ~iTween() {};
public:
    virtual bool Update(const float dt) = 0;
    virtual bool isComplete() const = 0;
    virtual float getAlpha() const = 0;
};

class TweenSystem : public iSubsystem
{
public:
    enum Easing {
        None = 0,
        LINEAR = 1
    };
public:
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
    TweenSystem& Get();
    std::shared_ptr<iTween> Create();
    /*
    tween.via(0, easing);
    tween.seek(0);
    for (int i = 0; i <= 100; i++) tween.step(0.01f);

    auto t = tweeny::from(0).to(100).during(100).onStep(print);
    bool print(tweeny::tween<int> &, int p) {
        printf("%+.3d |", p); // 3 digits with sign
        for (int i = 0; i <= 100; i++) printf("%c", i == p ? '.' : ' '); // prints the line
        printf("%c\n", p == 100 ? ';' : '|');
        return false;
    }

    */

private:
    std::vector<std::shared_ptr<iTween>> _tweens;
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

            //tween->_initial = start;
            //tween->_final = end;
        
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
        if(_duration != 0.0f)
            return _t / _duration;
        return 0.0f;
    }
private:
    std::function<bool(float, Tween& tween)> _onStart;
    std::function<bool(float, Tween& tween)> _onUpdate;
    std::function<bool(float, Tween& tween)> _onComplete;
    std::function<bool(float, Tween& tween)> _onCancel;
    float _t = 0;
    float _duration = 0;
};
