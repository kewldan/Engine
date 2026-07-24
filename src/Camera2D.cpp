#include "Camera2D.h"

Engine::Camera2D::Camera2D(Engine::Window *window) : window(window) {
    zoom = new Engine::Animation(0.5, 0.65f, 3.5f);
    zoom->start(1.f);
}

void Engine::Camera2D::update() {
    float z = zoom->getValue();
    if (z != lastZoom) {
        // keep the world point under the anchor fixed while the zoom animates
        position.x += (2.f * zoomAnchor.x - 1.f) * (float) window->width * (lastZoom - z);
        position.y += (2.f * zoomAnchor.y - 1.f) * (float) window->height * (lastZoom - z);
        lastZoom = z;
    }

    view = glm::translate(glm::mat4(1), -position);

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
    zoomAnchor = glm::vec2(0.5f, 0.5f); // centered zoom shifts nothing
    zoom->start(newZoom);
}

void Engine::Camera2D::zoomIn(float factor) {
    zoomAnchor = glm::vec2(0.5f, 0.5f);
    zoom->start(zoom->getTargetValue() + factor);
}

void Engine::Camera2D::zoomAt(float factor, glm::vec2 anchor) {
    zoomAnchor = anchor;
    zoom->start(zoom->getTargetValue() + factor);
}

Engine::Camera2D::~Camera2D() {
    delete zoom;
}
