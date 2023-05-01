#include "Camera.h"

Engine::Camera::Camera(Engine::Window *window) {
    this->window = window;
    position = glm::vec3(0);
    zoom = Engine::Animation(0.5, 0.65f, 3.5f);
    zoom.start(1.f);
    fov = Engine::Animation(0.1);
    setFov(60.f);
    updateView();
    updateOrthographic();
    updatePerspective();
}

glm::mat4 Engine::Camera::getView() {
    return view;
}

glm::mat4 Engine::Camera::getViewRotationOnly() {
    return viewRotation;
}

glm::mat4 Engine::Camera::getOrthographic() {
    return orthographic;
}

glm::mat4 Engine::Camera::getPerspective() {
    return perspective;
}

void Engine::Camera::update() {
    if (zoom.isRunning()) {
        updateOrthographic();
    }
    if(fov.isRunning()){
        updatePerspective();
    }
}

void Engine::Camera::updateView() {
    viewRotation = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    viewRotation = glm::rotate(viewRotation, rotation.y, glm::vec3(0, 1, 0));
    view = glm::translate(viewRotation, -position);
}

void Engine::Camera::updateOrthographic() {
    float z = zoom.getValue();
    left = (float) window->width * (1 - z);
    right = (float) window->width * z;
    top = (float) window->height * z;
    bottom = (float) window->height * (1 - z);
    orthographic = glm::ortho(left, right, bottom,
                              top, 0.01f, 10.0f);
}

void Engine::Camera::updatePerspective() {
    perspective = glm::perspective(fov.getValue(), (float) window->width / (float) window->height, 0.1f, 300.f);
}

void Engine::Camera::zoomIn(float factor) {
    zoom.start(zoom.getTargetValue() + factor);
}

void Engine::Camera::setZoom(float newZoom) {
    zoom.start(newZoom);
    updateView();
}

void Engine::Camera::setFov(float hFov) {
    ASSERT("Horizontal FOV < 0", hFov > 0);
    float vfovRad = 2.f * std::atan(std::tan(glm::radians(hFov) / 2) * (float) window->width / (float) window->height);
    fov.start(vfovRad);
}

float Engine::Camera::getZoom() {
    return zoom.getValue();
}
