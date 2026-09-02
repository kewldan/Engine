#pragma once

#include "glm/ext.hpp"
#include "Engine.h"
#include "plog/Log.h"
#include <GLFW/glfw3.h>
#include <array>

namespace Engine {
    class Input {
    private:
        // Keys below GLFW_KEY_SPACE (32) are never reported, so the tables start at 32.
        static constexpr int KEY_OFFSET = GLFW_KEY_SPACE;
        static constexpr int KEY_COUNT = GLFW_KEY_LAST - KEY_OFFSET + 1;
        static constexpr int MOUSE_BUTTON_COUNT = GLFW_MOUSE_BUTTON_LAST + 1;

        static Input *instance;
        // Callbacks that were installed before ours (e.g. ImGui's); we forward to them.
        static GLFWmousebuttonfun prevMouseButtonCallback;
        static GLFWkeyfun prevKeyCallback;
        static GLFWcursorposfun prevCursorPosCallback;
        static GLFWscrollfun prevScrollCallback;

        GLFWwindow *window;
        glm::vec2 cursorPosition{};
        glm::vec2 lastCursorPosition{};
        glm::vec2 draggingStartPosition{};
        glm::vec2 scrollDelta{};
        std::array<bool, KEY_COUNT> keyPressed{}, keyJustPressed{}, keyJustReleased{};
        std::array<bool, MOUSE_BUTTON_COUNT> mousePressed{}, mouseJustPressed{}, mouseJustReleased{};
        bool dragging{}, startDragging{}, stopDragging{};

        static void key_callback(int key, int action);

        static void mouse_callback(int button, int action);

        static void cursor_callback(float xpos, float ypos);

        static void mouse_scroll_callback(float x, float y);

        static bool isValidKey(int key);

        static bool isValidButton(int button);

    public:
        explicit Input(GLFWwindow *window);

        ~Input();

        Input(const Input &) = delete;

        Input &operator=(const Input &) = delete;

        void registerCallbacks();

        void hideCursor();

        void showCursor();

        glm::vec2 &getCursorPosition();

        void setCursorPosition(glm::vec2 position);

        bool update();

        [[nodiscard]] bool isKeyPressed(int key) const;

        [[nodiscard]] bool isKeyJustPressed(int key) const;

        [[nodiscard]] bool isKeyJustReleased(int key) const;

        [[nodiscard]] bool isMouseButtonPressed(int button) const;

        [[nodiscard]] bool isMouseButtonJustPressed(int button) const;

        [[nodiscard]] bool isMouseButtonJustReleased(int button) const;

        void setClipboard(const char *value);

        static bool isSupportRawMode();

        void setRawMode(bool value);

        [[nodiscard]] bool isStartDragging() const;

        [[nodiscard]] bool isDragging() const;

        [[nodiscard]] bool isStopDragging() const;

        glm::vec2 &getDraggingStartPosition();

        glm::vec2 &getMouseWheelDelta();

        const char *getClipboard();
    };
}
