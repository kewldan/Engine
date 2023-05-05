#pragma once

#include <glm/ext.hpp>
#include "Animation.h"

namespace Engine {
    class Camera2D {
    private:
        glm::mat4 view{}, orthographic{};
        Engine::Animation *zoom;
        float Z_NEAR = 0.01f, Z_FAR = 10.f;
    public:
        Engine::Window *window;
        float left{}, right{}, top{}, bottom{};
        glm::vec3 position{};

        explicit Camera2D(Window *window);

        void update();

        float getZoom();

        void setZoom(float newZoom);

        void zoomIn(float factor);

        void setProjectionDistance(float z_near, float z_far);

        const glm::mat4 &getView() const;

        const glm::mat4 &getProjection() const;
    };
}
