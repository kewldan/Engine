#include "Animation.h"

Engine::Animation::Animation(double duration, float minValue, float maxValue) : duration(duration), minValue(minValue),
                                                                                maxValue(maxValue) {

}

void Engine::Animation::start(float value) {
    startValue = getValue();
    startTime = glfwGetTime();
    targetValue = std::max(std::min(value, maxValue), minValue);
}

float Engine::Animation::getValue() {
    double now = glfwGetTime();
    if (now >= startTime + duration) {
        currentValue = targetValue;
    } else {
        // pure function of time, so multiple calls per frame stay consistent
        auto p = (float) ((now - startTime) / duration);
        p = p * p * (3.f - 2.f * p); // smoothstep
        currentValue = startValue + (targetValue - startValue) * p;
    }
    return currentValue;
}

float Engine::Animation::getTargetValue() const {
    return targetValue;
}

bool Engine::Animation::isRunning() const {
    return glfwGetTime() < startTime + duration;
}
