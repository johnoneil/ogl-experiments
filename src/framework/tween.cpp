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

}

bool TweenSystem::Startup() {
    return true;
}

bool TweenSystem::Shutdown() {
    return true;
}

void TweenSystem::Update(const float dt) {

}

void TweenSystem::Render() {

}

 std::function<float(float)> TweenSystem::GetEasingFunction(const Easing easing) {
    auto e = sEasingMap.find(easing);
    if(e != sEasingMap.end())
        return e->second;
    return LinearInterpolation;
 }

