#include "GUIUtils.h"


void show_help_marker(const char* desc){
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(450.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


void button_to_popup(const char * name, std::function<void()> popup_content){

    auto button_str = std::string{ name };
    button_str += "##Button";

    auto popup_str = std::string{ name };
    popup_str += "##Popup";

    if (ImGui::Button(button_str.c_str())) {
        ImGui::OpenPopup(popup_str.c_str());
    }

    if (ImGui::BeginPopupModal(popup_str.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        popup_content();

        ImGui::EndPopup();
    }

}
