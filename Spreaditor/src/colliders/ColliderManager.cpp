#include "ColliderManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "../render/RenderManager.h"
#include "../gui/GUIManager.h"
#include "../utils/GUIUtils.h"

ColliderManager::ColliderManager() {}

ColliderManager::~ColliderManager() {}

void ColliderManager::start_up() {

}

void ColliderManager::shut_down() {

}

void ColliderManager::draw_collider_gui() {

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

        INIT_GENERAL_ERROR_POPUP;

        for (auto& c : m_colliders) {

            auto& collider_type = c.first;
            auto& instances = c.second;

            auto col = ImVec4(
                collider_type.default_color.x,
                collider_type.default_color.y,
                collider_type.default_color.z,
                collider_type.default_color.w > ALPHA_LIMIT ? ALPHA_LIMIT : collider_type.default_color.w);

            auto col_active = col;
            auto col_hover = col;

            ImGui::PushStyleColor(ImGuiCol_Header, col);
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, col_active);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, col_hover);

            if (ImGui::CollapsingHeader(collider_type.name.c_str())) {

                if (collider_type.attributes.size() == 0) {
                    ImGui::Text("[%s] has no attributes", collider_type.name.c_str());
                }
                else {

                }

                ImGui::Separator();

                if (instances.size() == 0) {
                    ImGui::Text("[%s] has no intances", collider_type.name.c_str());
                }
                else {

                }

                //@@TODO: Show stuff for each collider type


            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

        }

        auto open_error = false;
        auto & colliders = m_colliders;
        button_to_popup("New Collider Type", [&]() {

            static char collider_type_name[MAX_COLLIDER_NAME_SIZE];

            static auto color = Vec4f();

            ImGui::Text("Name: ");
            ImGui::SameLine(100);
            ImGui::InputText("##ColliderTypeName", collider_type_name, MAX_COLLIDER_NAME_SIZE);
            ImGui::ColorEdit4("##ColliderTypeColor", (float*)&color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

            IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
            {
                ImGui::CloseCurrentPopup();
            }
            END_BUTTON_ALIGNED_RIGHT_FIRST;

            IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

                if (strlen(collider_type_name) == 0) {
                    SET_GENERAL_ERROR_POPUP_MESSAGE("The name is empty");
                    open_error = true;
                    CLOG("The name is empty");
                }
                else {

                    auto new_collider_type = create_collider_type(collider_type_name, color);

                    if (colliders.find(new_collider_type) != colliders.end()) {
                        SET_GENERAL_ERROR_POPUP_MESSAGE("The name for that Collider Type already exists: " << collider_type_name);
                        open_error = true;
                        CLOG("The name for that Collider Type already exists: " << collider_type_name);
                    }
                    else {
                        colliders.insert(std::make_pair(new_collider_type, InstanceContainer()));
                        ImGui::CloseCurrentPopup();
                    }
                }

            }
            END_BUTTON_ALIGNED_RIGHT_NEXT(accept);
        });

        if (open_error)
            OPEN_GENERAL_ERROR_POPUP;

        DEFINE_GENERAL_ERROR_POPUP;

        auto window_height = ImGui::GetCursorPos().y;
        auto window_width = ImGui::GetWindowWidth();
        ImGui::SetWindowPos(ImVec2(screen_size.x - window_width, padding_top), true);
    }
    ImGui::End();

}
