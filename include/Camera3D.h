#pragma once

#include "glm/ext.hpp"
#include "Animation.h"

namespace Engine {
    class Camera3D {
    private:
        glm::mat4 projection{}, view{}, viewRotation{};
        Engine::Animation *fov;
    public:
        Engine::Window *window;
        glm::vec3 position{};
        glm::vec2 rotation{};

        explicit Camera3D(Engine::Window *window);
        ~Camera3D();

        glm::mat4 &getView();

        glm::mat4 &getViewRotation();

        glm::mat4 &getProjection();

        void update();

        void setFov(float hFov);
    };
}
