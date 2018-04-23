#include "ToolsManager.h"
#include "../gui/GUIManager.h"
#include "../render/RenderManager.h"
#include "../colliders/ColliderManager.h"
#include "../sprites/SpriteManager.h"

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

				if (m_p_selected_instance)
					m_new_collider_color = ImColor(
						m_p_selected_instance->color.x,
						m_p_selected_instance->color.y,
						m_p_selected_instance->color.z,
						m_p_selected_instance->color.w);

			}

			m_new_collider.width = sprite_mouse_position.x;
			m_new_collider.height = sprite_mouse_position.y;


		}
		else if (m_dragging) { // We just finished collider creation

			m_currently_creating_collider = false;

			//@@TODO Save collider rect into the appropriate instance

			if (m_p_selected_type && m_p_selected_instance) {

				m_p_selected_instance->rects[SpriteManager::get().get_current_main_sprite_index()].push_back(m_new_collider);

				CLOG("Created new rect for instance [" << m_p_selected_instance->name << "] of type [" << m_p_selected_type->name << "]\n"
					"\tOrigin: ("
					<< m_new_collider.x << ", " << m_new_collider.y << ")\n"
					"\tSize: ("
					<< m_new_collider.width << ", " << m_new_collider.height << ")\n");

			}
			else {
				CLOG("No type has been selected to create the new rect\n"
					"\tOrigin: ("
					<< m_new_collider.x << ", " << m_new_collider.y << ")\n"
					"\tSize: ("
					<< m_new_collider.width << ", " << m_new_collider.height << ")\n");
			}


		}

		m_dragging = dragging;

	}


}

void ToolsManager::draw_tools_gui() {

	//// Check that our types and instances are correct
	auto & colliders = ColliderManager::get().get_non_const_colliders();
	{
		/*
		We check if our selected instances and types actually are correct and
		reference a real instance and type. If they don't we set them to nullptr.
		*/
		if (m_p_selected_type) {
			const auto & temp_type = colliders.find(m_selected_type_copy);
			if (temp_type != colliders.end()) {

				m_p_selected_type = &const_cast<ColliderType&>((*temp_type).first);

				auto & instances = temp_type->second;

				if (m_p_selected_instance) {
					const auto& temp_instance = instances.find(m_selected_instance_copy);
					if (temp_instance != instances.end()) {
						m_p_selected_instance = &const_cast<ColliderInstance&>(*temp_instance);
					}
					else {
						m_p_selected_instance = nullptr;
					}
				}
			}
			else {
				m_p_selected_type = nullptr;
				m_p_selected_instance = nullptr;
			}
		}
	}


	auto padding_top = GUIManager::get().get_main_menu_height();
	auto screen_size = RenderManager::get().get_main_render_target()->getSize();

	static auto resize_first_time = true;

	//// TOOL CONFIGURATION WINDOW
	{
		if (ImGui::Begin("##tool options window", nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoTitleBar
		)) {

			auto show_collider_selection = false;

			if (m_current_tool == Tool::NONE) {

				ImGui::Text("No Tool Selected");

			}
			else if (m_current_tool == Tool::CREATE_COLLIDER) {
				ImGui::Text("Create Rect Tool");
				show_collider_selection = true;

			}
			else if (m_current_tool == Tool::EDIT_COLLIDER) {

				ImGui::Text("Edit Collider Tool");
				show_collider_selection = true;


			}
			else {
				assert(false); // Unknown value for the selected tool
			}


			if (show_collider_selection) {


				//// Selecting Type for new rect
				ImGui::SameLineS();
				static const auto type_tag = std::string("Type: ");
				if (ImGui::BeginCombo("##ColliderTypeSelector", m_p_selected_type ? (type_tag + m_p_selected_type->name).c_str() : "No Type Selected")) {

					if (colliders.size() > 0) {

						for (const auto& pair : colliders) {

							auto& type = const_cast<ColliderType&>(pair.first);

							if (ImGui::Selectable(type.name.c_str(), m_p_selected_type && type == *m_p_selected_type)) {
								m_p_selected_type = &type;
							}

						}
					}
					else {
						ImGui::Text("No Types Defined");
					}
					ImGui::EndCombo();
				}

				//// Selecting Instance for new rect
				if (m_p_selected_type) {
					ImGui::SameLineS();
					static const auto instance_tag = std::string("Instance: ");
					if (ImGui::BeginCombo("##ColliderInstanceSelector", m_p_selected_instance ? (instance_tag + m_p_selected_instance->name).c_str() : "No Instance Selected")) {

						auto & instances = colliders.at(*m_p_selected_type);

						if (instances.size() > 0) {
							for (auto & instance : instances) {
								if (ImGui::Selectable(instance.name.c_str(), m_p_selected_instance && instance == *m_p_selected_instance)) {
									m_p_selected_instance = &const_cast<ColliderInstance&>(instance);
								}
							}
						}
						else {
							ImGui::Text("No Instances Defined");
						}
						ImGui::EndCombo();
					}
				}
			}


			ImGui::SameLineS();
			ImGui::Checkbox("Show All Rects", &m_show_all_rects);


			if (m_p_selected_type)
				m_selected_type_copy = *m_p_selected_type;

			/*
			@@TODO: Check why this can be invalid.

			It is invalid when we deleted all the attribute instances and that
			one was selected.
			*/
			if (m_p_selected_instance)
				m_selected_instance_copy = *m_p_selected_instance;


			ImGui::SameLineS();
			ImGui::Text("\t");


#pragma region modifying_tools_window

			// @@TODO: Maybe set here color of a menu bar

			auto window_size = ImGui::GetWindowSize();

			window_size.y = ImGui::GetTextLineHeight();

			ImGui::SetWindowSize(window_size);


			auto window_pos = ImGui::GetWindowPos();
			window_pos.x = 0;
			window_pos.y = GUIManager::get().get_main_menu_height();
			ImGui::SetWindowPos(window_pos);

			m_tools_options_bar_height = ImGui::GetWindowHeight();
			padding_top += m_tools_options_bar_height;

#pragma endregion modifying_tools_window

			ImGui::End();
		}
	}

	//// MAIN TOOL WINDOW
	{

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


#pragma region SizeAndPosWindow


			auto window_width = ImGui::GetWindowContentRegionWidth();
			if (window_width < ImGui::GetWindowWidth()) {
				window_width = ImGui::GetWindowWidth();
			}


			auto max_height = screen_size.y - padding_top - GUIManager::get().get_timeline_height();

			auto new_size = ImVec2(0, max_height);

			ImGui::SetWindowSize(new_size);

			ImGui::SetWindowPos(ImVec2(0, padding_top), true);

#pragma endregion SizeAndPosWindow

			m_tools_width = ImGui::GetWindowWidth();
		}
		ImGui::End();

	}

	//// DRAWING APPROPRIATE RECTS ON TOP OF SPRITE

	if (ImGui::Begin("##HiddenFullscreen", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoBringToFrontOnFocus))
	{

		// @@TODO: Check that we don't exceed the vertex limit of the draw_list for ImGui

		// We draw the colliders in any position of the window
		auto draw_list = ImGui::GetWindowDrawList();
		draw_list->PushClipRectFullScreen();

		auto current_sprite_index = SpriteManager::get().get_current_main_sprite_index();

		if (m_show_all_rects) {
			for (const auto & pair : colliders) {
				const auto & instances = pair.second;
				for (const auto & instance : instances) {

					draw_rects_of_instance(instance, current_sprite_index, draw_list);

				}
			}

		}
		else {

			if (m_p_selected_instance) {

				draw_rects_of_instance(*m_p_selected_instance, current_sprite_index, draw_list);

			}

		}


		draw_list->PopClipRect();

	}
	ImGui::End();

}

void ToolsManager::draw_rects_of_instance(const ColliderInstance & instance, int current_sprite_index, ImDrawList * draw_list) {

	auto border_color = ImColor(
		instance.color.x,
		instance.color.y,
		instance.color.z);

	auto fill_color = border_color;
	fill_color.Value.w = 0.4f;

	if (instance.rects.find(current_sprite_index) != instance.rects.end()) {
		for (const auto& rect : instance.rects.at(current_sprite_index)) {

			auto drawing_collider_rect_origin = GUIManager::get().sprite_to_global(sf::Vector2f(rect.x, rect.y));
			auto drawing_collider_rect_size = GUIManager::get().sprite_to_global(sf::Vector2f(rect.width, rect.height));

			draw_list->AddRectFilled(drawing_collider_rect_origin, drawing_collider_rect_size,
				ImGui::GetColorU32((ImVec4)fill_color));

			draw_list->AddRect(drawing_collider_rect_origin, drawing_collider_rect_size,
				ImGui::GetColorU32((ImVec4)border_color));
		}
	}

}
