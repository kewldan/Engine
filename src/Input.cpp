#include "Input.h"

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
}

void Engine::Input::showCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::vec2 Engine::Input::getCursorPosition() {
    return cursorPosition;
}

void Engine::Input::setCursorPosition(glm::vec2 position) {
    ASSERT("Cursor position out of bounds", position.x > 0 && position.y > 0);
    cursorPosition = position;
    glfwSetCursorPos(window, position.x, position.y);
}

void Engine::Input::update() {
    static double x, y;
    glfwGetCursorPos(window, &x, &y);
    cursorPosition.x = static_cast<float>(x);
    cursorPosition.y = static_cast<float>(y);

    for (int i = 0; i < 317; i++) {
        bool now = glfwGetKey(window, i + 32);
        if (now) {
            keyJustReleased[i] = false;
            keyJustPressed[i] = !keyPressed[i];
        } else {
            keyJustPressed[i] = false;
            keyJustReleased[i] = keyPressed[i];
        }
        keyPressed[i] = now;
    }

    for (int i = 0; i < 8; i++) {
        bool now = glfwGetMouseButton(window, i);
        if (now) {
            mouseJustReleased[i] = false;
            mouseJustPressed[i] = !mousePressed[i];
        } else {
            mouseJustPressed[i] = false;
            mouseJustReleased[i] = mousePressed[i];
        }
        mousePressed[i] = now;
    }
}

bool Engine::Input::isKeyPressed(int key) {
    return key >= 32 && key < 349 && keyPressed[key - 32];
}

bool Engine::Input::isKeyJustPressed(int key) {
    return key >= 32 && key < 349 && keyJustPressed[key - 32];
}

bool Engine::Input::isMouseButtonPressed(int button) {
    return button >= 0 && button < 8 && mousePressed[button];
}

bool Engine::Input::isMouseButtonJustPressed(int button) {
    return button >= 0 && button < 8 && mouseJustPressed[button];
}

void Engine::Input::setClipboard(const char *value) {
    ASSERT("Value for clipboard is nullptr", value != nullptr);
    glfwSetClipboardString(window, value);
}

const char *Engine::Input::getClipboard() {
    return glfwGetClipboardString(window);
}

bool Engine::Input::isKeyJustReleased(int key) {
    return key >= 32 && key < 349 && keyJustReleased[key];
}

bool Engine::Input::isMouseButtonJustReleased(int button) {
    return button >= 0 && button < 8 && mouseJustReleased[button];
}
