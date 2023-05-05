#include "Camera2D.h"

Engine::Camera2D::Camera2D(Engine::Window *window) : window(window) {
    zoom = new Engine::Animation(0.5, 0.65f, 3.5f);
    zoom->start(1.f);
}

void Engine::Camera2D::update() {
    view = glm::translate(glm::mat4(1), -position);

    float z = zoom->getValue();
    left = (float) window->width * (1 - z);
    right = (float) window->width * z;
    top = (float) window->height * z;
    bottom = (float) window->height * (1 - z);
    orthographic = glm::ortho(left, right, bottom,
                              top, Z_NEAR, Z_FAR);
}

float Engine::Camera2D::getZoom() {
    return zoom->getValue();
}

void Engine::Camera2D::setProjectionDistance(float z_near, float z_far) {
    Z_NEAR = z_near;
    Z_FAR = z_far;
}

const glm::mat4 &Engine::Camera2D::getView() const {
    return view;
}

const glm::mat4 &Engine::Camera2D::getProjection() const {
    return orthographic;
}

void Engine::Camera2D::setZoom(float newZoom) {
    zoom->start(newZoom);
}

void Engine::Camera2D::zoomIn(float factor) {
    zoom->start(zoom->getTargetValue() + factor);
}
