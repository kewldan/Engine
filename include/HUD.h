#pragma once

#include "imcmd_command_palette.h"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_notify.h"
#include "imfilebrowser.h"
#include <cmath>
#include "Window.h"

namespace Engine {
    class HUD {
    public:
        static bool show_command_palette;

        static void init(Engine::Window *window);

        static void destroy();

        static void begin();

        static void end();
    };
}
