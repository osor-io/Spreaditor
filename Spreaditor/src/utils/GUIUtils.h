#pragma once

#include "imgui.h"

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
if (ImGui::Button(text, size)) 

#define END_BUTTON_ALIGNED_RIGHT_FIRST first_button_width = ImGui::GetItemRectSize().x;

#define IF_BUTTON_ALIGNED_RIGHT_NEXT(text,size,ID)\
static float ID ## _button_width = size.x; \
pos -= spacing; \
pos -= ID ## _button_width; \
ImGui::SameLine(pos); \
if (ImGui::Button(text, size)) \

#define END_BUTTON_ALIGNED_RIGHT_NEXT(ID) ID ## _button_width = ImGui::GetItemRectSize().x;


