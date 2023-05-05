#include "Camera3D.h"

Engine::Camera3D::Camera3D(Engine::Window *window) : window(window) {
    fov = new Engine::Animation(0.1);
    setFov(60.f);
}

glm::mat4 &Engine::Camera3D::getView() {
    return view;
}

glm::mat4 &Engine::Camera3D::getProjection() {
    return projection;
}

void Engine::Camera3D::update() {
    view = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    view = glm::rotate(view, rotation.y, glm::vec3(0, 1, 0));
    view = glm::translate(view, -position);

    projection = glm::perspective(fov->getValue(), (float) window->width / (float) window->height, 0.1f, 300.f);
}

void Engine::Camera3D::setFov(float hFov) {
    ASSERT("Horizontal FOV < 0", hFov > 0);
    float vfovRad = 2.f * std::atan(std::tan(glm::radians(hFov) / 2) * (float) window->width / (float) window->height);
    fov->start(vfovRad);
}
