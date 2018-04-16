#include "ColliderManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "../render/RenderManager.h"
#include "../gui/GUIManager.h"


ColliderManager::ColliderManager() {}

ColliderManager::~ColliderManager() {}

void ColliderManager::start_up() {

}

void ColliderManager::shut_down() {

}

void ColliderManager::draw_collider_gui(){

    auto padding_top = GUIManager::get().get_main_menu_height();
    auto screen_size = RenderManager::get().get_main_render_target()->getSize();
    ImGui::SetNextWindowSize(ImVec2(0, /* @@TODO: Set here the height we want */0));
    if (ImGui::Begin("Collider Explorer", nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoTitleBar
    )) {


        ImGui::Text("Collider Explorer");

        

        auto window_height = ImGui::GetCursorPos().y;
        auto window_width = ImGui::GetWindowWidth();
        ImGui::SetWindowPos(ImVec2(screen_size.x - window_width, padding_top), true);
    }
    ImGui::End();

}
