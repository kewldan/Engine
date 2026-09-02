#include "Input.h"

Engine::Input *Engine::Input::instance = nullptr;
GLFWmousebuttonfun Engine::Input::prevMouseButtonCallback = nullptr;
GLFWkeyfun Engine::Input::prevKeyCallback = nullptr;
GLFWcursorposfun Engine::Input::prevCursorPosCallback = nullptr;
GLFWscrollfun Engine::Input::prevScrollCallback = nullptr;

void Engine::Input::hideCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Engine::Input::Input(GLFWwindow *window) : window(window) {
    ASSERT("Window is nullptr", window != nullptr);
}

void Engine::Input::showCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::vec2 &Engine::Input::getCursorPosition() {
    return cursorPosition;
}

void Engine::Input::setCursorPosition(glm::vec2 position) {
    ASSERT("Cursor position out of bounds", position.x >= 0 && position.y >= 0);
    cursorPosition = position;
    glfwSetCursorPos(window, position.x, position.y);
}

bool Engine::Input::update() {
    mouseJustPressed.fill(false);
    mouseJustReleased.fill(false);

    keyJustPressed.fill(false);
    keyJustReleased.fill(false);

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

bool Engine::Input::isValidKey(int key) {
    return key >= KEY_OFFSET && key <= GLFW_KEY_LAST;
}

bool Engine::Input::isValidButton(int button) {
    return button >= 0 && button < MOUSE_BUTTON_COUNT;
}

bool Engine::Input::isKeyPressed(int key) const {
    ASSERT("Key is invalid", isValidKey(key));
    return isValidKey(key) && keyPressed[key - KEY_OFFSET];
}

bool Engine::Input::isKeyJustPressed(int key) const {
    ASSERT("Key is invalid", isValidKey(key));
    return isValidKey(key) && keyJustPressed[key - KEY_OFFSET];
}

bool Engine::Input::isMouseButtonPressed(int button) const {
    ASSERT("Button is invalid", isValidButton(button));
    return isValidButton(button) && mousePressed[button];
}

bool Engine::Input::isMouseButtonJustPressed(int button) const {
    ASSERT("Button is invalid", isValidButton(button));
    return isValidButton(button) && mouseJustPressed[button];
}

void Engine::Input::setClipboard(const char *value) {
    ASSERT("Value for clipboard is nullptr", value != nullptr);
    glfwSetClipboardString(window, value);
}

const char *Engine::Input::getClipboard() {
    return glfwGetClipboardString(window);
}

bool Engine::Input::isKeyJustReleased(int key) const {
    ASSERT("Key is invalid", isValidKey(key));
    return isValidKey(key) && keyJustReleased[key - KEY_OFFSET];
}

bool Engine::Input::isMouseButtonJustReleased(int button) const {
    ASSERT("Button is invalid", isValidButton(button));
    return isValidButton(button) && mouseJustReleased[button];
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

// `key` is already offset by KEY_OFFSET here.
void Engine::Input::key_callback(int key, int action) {
    if (action != GLFW_RELEASE) {
        Engine::Input::instance->keyJustPressed[key] = !Engine::Input::instance->keyPressed[key];
    } else {
        Engine::Input::instance->keyJustReleased[key] = Engine::Input::instance->keyPressed[key];
    }
    Engine::Input::instance->keyPressed[key] = action != GLFW_RELEASE;
}

void Engine::Input::mouse_callback(int button, int action) {
    if (isValidButton(button)) {
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
    Engine::Input::instance->cursorPosition.x = xpos;
    Engine::Input::instance->cursorPosition.y = ypos;
}

void Engine::Input::registerCallbacks() {
    Engine::Input::instance = this;
    // glfwSet*Callback returns the previously installed callback (ImGui's, if HUD::init ran first);
    // keep it and forward, so both ImGui and Input receive events regardless of init order.
    prevMouseButtonCallback = glfwSetMouseButtonCallback(window, [](GLFWwindow *w, int button, int action, int mods) {
        if (prevMouseButtonCallback) prevMouseButtonCallback(w, button, action, mods);
        if (instance) mouse_callback(button, action);
    });
    prevKeyCallback = glfwSetKeyCallback(window, [](GLFWwindow *w, int key, int scancode, int action, int mods) {
        if (prevKeyCallback) prevKeyCallback(w, key, scancode, action, mods);
        if (instance && isValidKey(key)) {
            key_callback(key - KEY_OFFSET, action);
        }
    });
    prevCursorPosCallback = glfwSetCursorPosCallback(window, [](GLFWwindow *w, double xpos, double ypos) {
        if (prevCursorPosCallback) prevCursorPosCallback(w, xpos, ypos);
        if (instance) cursor_callback((float) xpos, (float) ypos);
    });
    prevScrollCallback = glfwSetScrollCallback(window, [](GLFWwindow *w, double xOffset, double yOffset) {
        if (prevScrollCallback) prevScrollCallback(w, xOffset, yOffset);
        if (instance) mouse_scroll_callback((float) xOffset, (float) yOffset);
    });
}

void Engine::Input::mouse_scroll_callback(float x, float y) {
    Engine::Input::instance->scrollDelta.x += x;
    Engine::Input::instance->scrollDelta.y += y;
}

glm::vec2 &Engine::Input::getMouseWheelDelta() {
    return scrollDelta;
}

bool Engine::Input::isSupportRawMode() {
    return glfwRawMouseMotionSupported();
}

void Engine::Input::setRawMode(bool value) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, value ? GLFW_TRUE : GLFW_FALSE);
}

Engine::Input::~Input() {
    // The GLFW callbacks stay installed (the window may already be gone), but they must not
    // touch a destroyed Input.
    if (instance == this) {
        instance = nullptr;
    }
}
