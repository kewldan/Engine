#pragma once

#include "glm/ext.hpp"
#include "Engine.h"
#include <GLFW/glfw3.h>

namespace Engine {
    class Input {
    private:
        GLFWwindow *window;
        glm::vec2 cursorPosition{};
        bool *keyPressed, *mousePressed;
        bool *keyJustPressed, *mouseJustPressed;
        bool *keyJustReleased, *mouseJustReleased;
    public:
        explicit Input(GLFWwindow *window);

        void hideCursor();

        void showCursor();

        glm::vec2 getCursorPosition();

        void setCursorPosition(glm::vec2 position);

        void update();

        bool isKeyPressed(int key);

        bool isKeyJustPressed(int key);

        bool isKeyJustReleased(int key);

        bool isMouseButtonPressed(int button);

        bool isMouseButtonJustPressed(int button);

        bool isMouseButtonJustReleased(int button);

        void setClipboard(const char* value);

        const char* getClipboard();
    };
}