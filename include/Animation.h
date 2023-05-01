#pragma once

#include "Window.h"
#include <cmath>

namespace Engine {
    class Animation {
    private:
        float currentValue{}, targetValue{}, minValue{}, maxValue{};
        double startTime{}, duration{};
    public:
        explicit Animation(double duration = 0.5, float minValue = -99999.f, float maxValue = 99999.f);

        void start(float value);

        float getValue();

        [[nodiscard]] float getTargetValue() const;

        [[nodiscard]] bool isRunning() const;
    };
}