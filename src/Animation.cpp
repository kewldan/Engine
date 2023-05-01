#include "Animation.h"

Engine::Animation::Animation(double duration, float minValue, float maxValue) : duration(duration), minValue(minValue),
                                                                                maxValue(maxValue) {

}

void Engine::Animation::start(float value) {
    startTime = glfwGetTime();
    targetValue = value;
    targetValue = std::max(std::min(targetValue, maxValue), minValue);
}

float Engine::Animation::getValue() {
    if (glfwGetTime() < startTime + duration) {
        currentValue = std::lerp(currentValue, targetValue, (float) ((glfwGetTime() - startTime) / duration));
    } else {
        currentValue = targetValue;
    }
    return currentValue;
}

float Engine::Animation::getTargetValue() const {
    return targetValue;
}

bool Engine::Animation::isRunning() const {
    return glfwGetTime() < startTime + duration;
}
