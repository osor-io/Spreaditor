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

void ToolsManager::tick() {

	auto dragging = ImGui::IsMouseDragging();


	auto sprite_inside_sprite = GUIManager::get().is_mouse_inside_sprite();
	auto sprite_mouse_position = GUIManager::get().get_sprite_mouse_pos();

	if (m_current_tool == Tool::CREATE_COLLIDER && sprite_inside_sprite) {

		if (dragging) {

			if (!m_dragging) { // We just clicked to create a new collider

				m_currently_creating_collider = true;

				m_new_collider.x = sprite_mouse_position.x;
				m_new_collider.y = sprite_mouse_position.y;

			}

			m_new_collider.width = sprite_mouse_position.x;
			m_new_collider.height = sprite_mouse_position.y;


		}
		else if (m_dragging) { // We just finished collider creation

			m_currently_creating_collider = false;

			//@@TODO Save collider rect into the appropriate instance

			CLOG("Created New Rect\n"
				"\tOrigin: ("
				<< m_new_collider.x << ", " << m_new_collider.y << ")\n"
				"\tSize: ("
				<< m_new_collider.width << ", " << m_new_collider.height << ")\n")


		}

		m_dragging = dragging;

	}


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

#pragma region Tools


		const auto scaling = GUIManager::get().scaling();

		if (ImGui::Selectable("New", m_current_tool == Tool::CREATE_COLLIDER, 0, ImVec2(30 * scaling, 30 * scaling)))
		{
			m_current_tool = Tool::CREATE_COLLIDER;
		}

		if (ImGui::Selectable("Edit", m_current_tool == Tool::EDIT_COLLIDER, 0, ImVec2(30 * scaling, 30 * scaling)))
		{
			m_current_tool = Tool::EDIT_COLLIDER;
		}


#pragma endregion Tools


#pragma region Drawing

		auto draw_list = ImGui::GetWindowDrawList();
		draw_list->PushClipRectFullScreen();

		if (m_currently_creating_collider) {


			auto drawing_collider_rect_origin = GUIManager::get().sprite_to_global(sf::Vector2f(m_new_collider.x, m_new_collider.y));
			auto drawing_collider_rect_size = GUIManager::get().sprite_to_global(sf::Vector2f(m_new_collider.width, m_new_collider.height));


			draw_list->AddRect(drawing_collider_rect_origin, drawing_collider_rect_size,
				ImGui::GetColorU32((ImVec4)m_new_collider_color));
		}
		draw_list->PopClipRect();


#pragma endregion Drawing


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
