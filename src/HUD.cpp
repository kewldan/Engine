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
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::HUD::init(Engine::Window *window) {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->getId(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::GetIO().IniFilename = nullptr;
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 6.f;
    style.FrameRounding = 4.f;
}

void Engine::HUD::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}
