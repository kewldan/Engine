#include "HUD.h"

void Engine::HUD::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Engine::HUD::end() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
    ImGui::RenderNotifications();
    ImGui::PopStyleColor(1);
    if (show_command_palette) {
        ImCmd::CommandPaletteWindow("CommandPalette", &show_command_palette);
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::HUD::init(Engine::Window *window) {
    ImGui::CreateContext();
    ImCmd::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->getId(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO &io = ImGui::GetIO();

    io.IniFilename = nullptr;
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 6.f;
    style.FrameRounding = 4.f;

    ImCmd::Command select_theme_cmd;
    select_theme_cmd.Name = "Select theme";
    select_theme_cmd.InitialCallback = [&]() {
        ImCmd::Prompt(std::vector<std::string>{
                "Classic",
                "Dark",
                "Light",
        });
    };
    select_theme_cmd.SubsequentCallback = [&](int selected_option) {
        switch (selected_option) {
            case 0:
                ImGui::StyleColorsClassic();
                break;
            case 1:
                ImGui::StyleColorsDark();
                break;
            case 2:
                ImGui::StyleColorsLight();
                break;
            default:
                break;
        }
    };
    ImCmd::AddCommand(std::move(select_theme_cmd));
}

void Engine::HUD::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

bool Engine::HUD::show_command_palette = false;
