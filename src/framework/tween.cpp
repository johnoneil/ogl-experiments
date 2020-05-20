#include "framework/tween.h"
#include "easing.h"

#include <unordered_map>

// We need a separate functor to generate hashes of our easing enum
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

static std::unordered_map<TweenSystem::Easing, std::function<float(float)>, EnumClassHash> sEasingMap = {
    { TweenSystem::Easing::NONE, LinearInterpolation},
    { TweenSystem::Easing::LINEAR, LinearInterpolation },
    { TweenSystem::Easing::QUADRATIC_IN, QuadraticEaseIn },
    { TweenSystem::Easing::QUADRATIC_OUT, QuadraticEaseOut },
    { TweenSystem::Easing::QUADRATIC_INOUT, QuadraticEaseInOut },
    { TweenSystem::Easing::CUBIC_IN, CubicEaseIn },
    { TweenSystem::Easing::CUBIC_OUT, CubicEaseOut },
    { TweenSystem::Easing::CUBIC_INOUT, CubicEaseInOut },
    { TweenSystem::Easing::QUARTIC_IN, QuarticEaseIn },
    { TweenSystem::Easing::QUARTIC_OUT, QuarticEaseOut },
    { TweenSystem::Easing::QUARTIC_INOUT, QuarticEaseInOut },
    { TweenSystem::Easing::QUINTIC_IN, QuinticEaseIn },
    { TweenSystem::Easing::QUINTIC_OUT, QuinticEaseOut },
    { TweenSystem::Easing::QUINTIC_INOUT, QuinticEaseInOut },
    { TweenSystem::Easing::SIN_IN, SineEaseIn },
    { TweenSystem::Easing::SIN_OUT, SineEaseOut },
    { TweenSystem::Easing::SIN_INOUT, SineEaseInOut },
    { TweenSystem::Easing::CIRCULAR_IN, CircularEaseIn },
    { TweenSystem::Easing::CIRCULAR_OUT, CircularEaseOut },
    { TweenSystem::Easing::CIRCULAR_INOUT, CircularEaseInOut },
    { TweenSystem::Easing::EXPONENTIAL_IN, ExponentialEaseIn },
    { TweenSystem::Easing::EXPONENTIAL_OUT, ExponentialEaseOut },
    { TweenSystem::Easing::EXPONENTIAL_INOUT, ExponentialEaseInOut },
    { TweenSystem::Easing::ELASTIC_IN, ElasticEaseIn },
    { TweenSystem::Easing::ELASTIC_OUT, ElasticEaseOut },
    { TweenSystem::Easing::ELASTIC_INOUT, ElasticEaseInOut },
    { TweenSystem::Easing::BACK_IN, BackEaseIn },
    { TweenSystem::Easing::BACK_OUT, BackEaseOut },
    { TweenSystem::Easing::BACK_INOUT, BackEaseInOut },
    { TweenSystem::Easing::BOUNCE_IN, BounceEaseIn },
    { TweenSystem::Easing::BOUNCE_OUT, BounceEaseOut },
    { TweenSystem::Easing::BOUNCE_INOUT, BounceEaseInOut }
};

TweenSystem::TweenSystem() {

}

#if 0
TweenSystem::TweenSystem(const TweenSystem& other) {

}

TweenSystem& TweenSystem::operator=(const TweenSystem& other) {

}
#endif

TweenSystem::~TweenSystem() {
    Shutdown();
}

bool TweenSystem::Startup() {
    assert(!_initialized);
    _initialized = true;
    return _initialized;
}

bool TweenSystem::Shutdown() {
    _initialized = false;
    for(auto t : _tweens) {
        t->Cancel();
    }
    _tweens.clear();
    return !_initialized;
}

void TweenSystem::Update(const float dt) {
    if(!_initialized) return;
    for(auto t = _tweens.begin(); t != _tweens.end();) {
        if((*t)->Update(dt)) {
            t = _tweens.erase(t);
        } else {
            ++t;
        }
    }
}

void TweenSystem::Render() {
    return;
}

void TweenSystem::AddTween(std::shared_ptr<iTween> tween, bool start/*=false*/) {
    // TODO: check for duplicates etc
    _tweens.push_back(tween);
    if(start)
        tween->Start();
}

void RemoveTween(std::shared_ptr<iTween> tween) {
    // TODO:
}

TweenSystem& TweenSystem::Get() {
    static TweenSystem system;
    return system;
}

 std::function<float(float)> TweenSystem::GetEasingFunction(const Easing easing) {
    auto e = sEasingMap.find(easing);
    if(e != sEasingMap.end())
        return e->second;
    return LinearInterpolation;
 }

 std::shared_ptr<iTween> DummyTween(const float duration) {
    auto tween = std::make_shared<TweenDummy>(duration);
    TweenSystem::Get().AddTween(tween);
    return tween;
}

std::shared_ptr<iTween> Pause(const float duration) {
    return DummyTween(duration);
}

#if 0
std::shared_ptr<iTween> DummyTween(const float duration, std::function<void(void)> onComplete /* = nullptr*/) {
    auto tween = Tween::Create(duration, TweenSystem::Easing::LINEAR,
    nullptr, // onStart
    [=](float dt, iTween& tween)->bool{ // onUpdate
        return tween.getAlpha() >= 1.0f;
    },
    [onComplete](float t, iTween& tween)->bool { // onComplete
        if(onComplete)
            onComplete();
        return false;
    },
    nullptr); // onCancel

    return tween;
}
std::shared_ptr<iTween> Pause(const float duration) { return DummyTween(duration);}
#endif

