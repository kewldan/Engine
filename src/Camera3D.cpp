#include "Camera3D.h"

Engine::Camera3D::Camera3D(Engine::Window *window) : fov(0.1), window(window) {
    ASSERT("Window is nullptr", window != nullptr);
    setFov(60.f);
}

glm::mat4 &Engine::Camera3D::getView() {
    return view;
}

glm::mat4 &Engine::Camera3D::getProjection() {
    return projection;
}

float Engine::Camera3D::getAspect() const {
    // A minimised window reports a 0x0 framebuffer; glm::perspective asserts on a zero aspect.
    if (window->width <= 0 || window->height <= 0) {
        return 1.f;
    }
    return (float) window->width / (float) window->height;
}

void Engine::Camera3D::update() {
    viewRotation = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    viewRotation = glm::rotate(viewRotation, rotation.y, glm::vec3(0, 1, 0));
    view = glm::translate(viewRotation, -position);

    projection = glm::perspective(fov.getValue(), getAspect(), 0.1f, 300.f);
}

void Engine::Camera3D::setFov(float hFov) {
    ASSERT("Horizontal FOV <= 0", hFov > 0);
    float vfovRad = 2.f * std::atan(std::tan(glm::radians(hFov) / 2) * getAspect());
    fov.start(vfovRad);
}

glm::mat4 &Engine::Camera3D::getViewRotation() {
    return viewRotation;
}
