#include "Input.h"

Engine::Input *Engine::Input::instance = nullptr;

void Engine::Input::hideCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Engine::Input::Input(GLFWwindow *window) : window(window) {
    ASSERT("Window is nullptr", window != nullptr);
    keyPressed = new bool[318];
    keyJustPressed = new bool[318];
    keyJustReleased = new bool[318];
    mousePressed = new bool[8];
    mouseJustPressed = new bool[8];
    mouseJustReleased = new bool[8];
    dragging = false;
    stopDragging = false;
    startDragging = false;

    memset(keyPressed, 0, 318);
    memset(mousePressed, 0, 8);
}

void Engine::Input::showCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::vec2 &Engine::Input::getCursorPosition() {
    return cursorPosition;
}

void Engine::Input::setCursorPosition(glm::vec2 position) {
    ASSERT("Cursor position out of bounds", position.x > 0 && position.y > 0);
    cursorPosition = position;
    glfwSetCursorPos(window, position.x, position.y);
}

bool Engine::Input::update() {
    memset(mouseJustPressed, 0, 8);
    memset(mouseJustReleased, 0, 8);

    memset(keyJustPressed, 0, 318);
    memset(keyJustReleased, 0, 318);

    scrollDelta = glm::vec2(0);

    glfwPollEvents();

    startDragging = false;
    stopDragging = false;

    if (isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && isKeyPressed(GLFW_KEY_LEFT_SHIFT) &&
        lastCursorPosition != cursorPosition && !dragging) {
        startDragging = true;
        dragging = true;
        draggingStartPosition = cursorPosition;
    }

    if ((isMouseButtonJustReleased(GLFW_MOUSE_BUTTON_LEFT) || isKeyJustReleased(GLFW_KEY_LEFT_SHIFT)) && dragging) {
        dragging = false;
        stopDragging = true;
    }
    return true;
}

bool Engine::Input::isKeyPressed(int key) {
    ASSERT("Key is invalid", key > 31 && key < 349);
    return keyPressed[key - 32];
}

bool Engine::Input::isKeyJustPressed(int key) {
    ASSERT("Key is invalid", key > 31 && key < 349);
    return keyJustPressed[key - 32];
}

bool Engine::Input::isMouseButtonPressed(int button) {
    ASSERT("Button is invalid", button >= 0 && button < 8);
    return mousePressed[button];
}

bool Engine::Input::isMouseButtonJustPressed(int button) {
    ASSERT("Button is invalid", button >= 0 && button < 8);
    return mouseJustPressed[button];
}

void Engine::Input::setClipboard(const char *value) {
    ASSERT("Value for clipboard is nullptr", value != nullptr);
    glfwSetClipboardString(window, value);
}

const char *Engine::Input::getClipboard() {
    return glfwGetClipboardString(window);
}

bool Engine::Input::isKeyJustReleased(int key) {
    ASSERT("Key is invalid", key > 31 && key < 349);
    return keyJustReleased[key - 32];
}

bool Engine::Input::isMouseButtonJustReleased(int button) {
    ASSERT("Button is invalid", button >= 0 && button < 8);
    return mouseJustReleased[button];
}

bool Engine::Input::isDragging() const {
    return dragging;
}

bool Engine::Input::isStartDragging() const {
    return startDragging;
}

bool Engine::Input::isStopDragging() const {
    return stopDragging;
}

glm::vec2 &Engine::Input::getDraggingStartPosition() {
    return draggingStartPosition;
}

void Engine::Input::key_callback(int key, int action) {
    if (action != GLFW_RELEASE) {
        Engine::Input::instance->keyJustPressed[key] = !Engine::Input::instance->keyPressed[key];
    } else {
        Engine::Input::instance->keyJustReleased[key] = Engine::Input::instance->keyPressed[key];
    }
    Engine::Input::instance->keyPressed[key] = action != GLFW_RELEASE;
}

void Engine::Input::mouse_callback(int button, int action) {
    if (button >= 0 && button < 8) {
        if (action != GLFW_RELEASE) {
            Engine::Input::instance->mouseJustPressed[button] = !Engine::Input::instance->mousePressed[button];
        } else {
            Engine::Input::instance->mouseJustReleased[button] = Engine::Input::instance->mousePressed[button];
        }
        Engine::Input::instance->mousePressed[button] = action != GLFW_RELEASE;
    }
}

void Engine::Input::cursor_callback(float xpos, float ypos) {
    Engine::Input::instance->lastCursorPosition = Engine::Input::instance->cursorPosition;
    Engine::Input::instance->cursorPosition.x = (float) xpos;
    Engine::Input::instance->cursorPosition.y = (float) ypos;
}

void Engine::Input::registerCallbacks() {
    Engine::Input::instance = this;
    glfwSetMouseButtonCallback(window, [](GLFWwindow *, int button, int action, int) {
        mouse_callback(button, action);
    });
    glfwSetKeyCallback(window, [](GLFWwindow *, int key, int, int action, int) {
        key_callback(key - 32, action);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *, double xpos, double ypos) {
        cursor_callback((float) xpos, (float) ypos);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *, double xOffset, double yOffset) {
        mouse_scroll_callback((float) xOffset, (float) yOffset);
    });
}

void Engine::Input::mouse_scroll_callback(float x, float y) {
    Engine::Input::instance->scrollDelta.x = x;
    Engine::Input::instance->scrollDelta.y = y;
}

glm::vec2 &Engine::Input::getMouseWheelDelta() {
    return scrollDelta;
}
