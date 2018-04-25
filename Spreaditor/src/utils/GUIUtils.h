#pragma once

#include "imgui.h"

#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

#define BEGIN_MENU_POPUP_MODAL(name)\
if (ImGui::Button(name)) {\
    CLOG("Opened \"" name "\" Pop Up");\
    ImGui::OpenPopup( name "##Modal");\
}\
if (ImGui::BeginPopupModal(name "##Modal", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

#define END_MENU_POPUP_MODAL  ImGui::EndPopup();}

#define IF_BUTTON_ALIGNED_RIGHT_FIRST(text,size)\
const auto spacing = ImGui::GetStyle().ItemSpacing.x; \
const auto padding = ImGui::GetStyle().WindowPadding.x; \
static float first_button_width = size.x; \
auto pos = ImGui::GetWindowContentRegionWidth() - first_button_width + padding; \
ImGui::SetCursorPosX(pos); \
if (MyImGui::Button(text, size)) 

#define END_BUTTON_ALIGNED_RIGHT_FIRST first_button_width = ImGui::GetItemRectSize().x;

#define IF_BUTTON_ALIGNED_RIGHT_NEXT(text,size,ID)\
static float ID ## _button_width = size.x; \
pos -= spacing; \
pos -= ID ## _button_width; \
ImGui::SameLine(pos); \
if (MyImGui::Button(text, size)) \

#define END_BUTTON_ALIGNED_RIGHT_NEXT(ID) ID ## _button_width = ImGui::GetItemRectSize().x;







#define INIT_GENERAL_ERROR_POPUP \
static auto error_message = std::string{};

#define SET_GENERAL_ERROR_POPUP_MESSAGE(msg)\
{auto ss = std::stringstream{}; ss << msg; error_message = ss.str();}

#define ERROR_POPUP_WIDTH 400

#define DEFINE_GENERAL_ERROR_POPUP {\
ImGui::SetNextWindowSize(ImVec2(ERROR_POPUP_WIDTH,0)); \
if (ImGui::BeginPopupModal("Error##GeneralErrorPopup", nullptr /*, ImGuiWindowFlags_ResizeFromAnySide*/)) { \
ImGui::TextWrapped("Ooops! %s", error_message.c_str());\
IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))\
{\
ImGui::CloseCurrentPopup();\
}\
END_BUTTON_ALIGNED_RIGHT_FIRST;\
ImGui::EndPopup();\
}}

#define OPEN_GENERAL_ERROR_POPUP \
ImGui::OpenPopup("Error##GeneralErrorPopup");




std::string generate_edit_rect_name(const std::string& type, const std::string& instance, int sprite_index, int rect_index);

void show_help_marker(const char* desc);

void button_to_popup(const char* name, std::function<void()> popup_content, std::function<void()> on_button = []() {});