#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Window.h"
#include "Animation.h"
#include <cmath>

namespace Engine {
    class Camera {
    private:
        glm::mat4 view{}, viewRotation{}, orthographic{}, perspective{};
        Engine::Animation zoom, fov;
    public:
        Window *window;
        float left{}, right{}, top{}, bottom{};
        glm::vec3 position{};
        glm::vec2 rotation{};

        explicit Camera(Window *window);

        glm::mat4 getView();

        glm::mat4 getViewRotationOnly();

        glm::mat4 getOrthographic();

        glm::mat4 getPerspective();

        void update();

        void updateView();

        void updateOrthographic();

        void updatePerspective();

        void zoomIn(float factor);

        void setZoom(float newZoom);

        float getZoom();

        void setFov(float hFov);
    };
}
