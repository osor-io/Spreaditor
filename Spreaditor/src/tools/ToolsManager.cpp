#include "ToolsManager.h"
#include "../gui/GUIManager.h"
#include "../render/RenderManager.h"

ToolsManager::ToolsManager() {
}


ToolsManager::~ToolsManager() {
}

void ToolsManager::start_up() {

}

void ToolsManager::shut_down() {

}

void ToolsManager::draw_tools_gui() {

	auto padding_top = GUIManager::get().get_main_menu_height();
	auto screen_size = RenderManager::get().get_main_render_target()->getSize();

	static auto resize_first_time = true;


	if (ImGui::Begin("Tools Explorer", nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoTitleBar
	)) {


		ImGui::Button("HOLA!");


#pragma region ResizeWindow


		auto window_width = ImGui::GetWindowContentRegionWidth();
		if (window_width < ImGui::GetWindowWidth()) {
			window_width = ImGui::GetWindowWidth();
		}


		auto max_height = screen_size.y - GUIManager::get().get_main_menu_height() - GUIManager::get().get_timeline_height();

		auto new_size = ImVec2(0, max_height);

		ImGui::SetWindowSize(new_size);

		ImGui::SetWindowPos(ImVec2(0, padding_top), true);

#pragma endregion ResizeWindow

		m_tools_width = ImGui::GetWindowWidth();
	}
	ImGui::End();

}
