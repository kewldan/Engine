#pragma once

#include "glm/ext.hpp"
#include "Engine.h"
#include "plog/Log.h"
#include <GLFW/glfw3.h>

namespace Engine {
    class Input {
    private:
        static Input *instance;
        GLFWwindow *window;
        glm::vec2 cursorPosition{};
        glm::vec2 lastCursorPosition{};
        glm::vec2 draggingStartPosition{};
        glm::vec2 scrollDelta{};
        bool *keyPressed, *mousePressed;
        bool *keyJustPressed, *mouseJustPressed;
        bool *keyJustReleased, *mouseJustReleased;
        bool dragging, startDragging, stopDragging;

        static void key_callback(int key, int action);

        static void mouse_callback(int button, int action);

        static void cursor_callback(float xpos, float ypos);

        static void mouse_scroll_callback(float x, float y);

    public:
        explicit Input(GLFWwindow *window);

        void registerCallbacks();

        void hideCursor();

        void showCursor();

        glm::vec2 &getCursorPosition();

        void setCursorPosition(glm::vec2 position);

        bool update();

        bool isKeyPressed(int key);

        bool isKeyJustPressed(int key);

        bool isKeyJustReleased(int key);

        bool isMouseButtonPressed(int button);

        bool isMouseButtonJustPressed(int button);

        bool isMouseButtonJustReleased(int button);

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