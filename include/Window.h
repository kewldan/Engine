#pragma once

#include "Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include "Engine.h"

#ifndef NDEBUG

#include "plog/Initializers/ConsoleInitializer.h"
#include "plog/Formatters/FuncMessageFormatter.h"

#endif

#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Engine {
    void error_callback(int code, const char *message);

    class Window {
    private:
        GLFWwindow *window{nullptr};
        bool resized{};
        bool vsync{};
    public:
        int width, height;

        static void init();

        static void destroy();

        explicit Window(int w = 1280, int h = 720, const char *title = "Untitled");

        ~Window();

        // Owns a GLFWwindow: copying would destroy it twice.
        Window(const Window &) = delete;

        Window &operator=(const Window &) = delete;

        void setVsync(bool value);

        void setTitle(const char *title);

        bool isResized();

        void reset() const;

        GLFWwindow *getId();

        bool update();

        void setIcon(const char *path);
    };
}
