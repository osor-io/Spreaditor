#include "ToolsManager.h"
#include "../gui/GUIManager.h"
#include "../render/RenderManager.h"
#include "../colliders/ColliderManager.h"
#include "../sprites/SpriteManager.h"
#include "GUIUtils.h"
#include <limits>

#define INDEX_THRESHOLD_TO_ADD 256

static const auto hidden_flags = ImGuiWindowFlags_NoTitleBar |
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoScrollbar |
ImGuiWindowFlags_NoInputs |
ImGuiWindowFlags_NoSavedSettings |
ImGuiWindowFlags_NoFocusOnAppearing |
ImGuiWindowFlags_NoBringToFrontOnFocus;

ToolsManager::ToolsManager() {
}


ToolsManager::~ToolsManager() {
}

void ToolsManager::start_up() {

	auto& style = m_edit_collider_style;

	style.WindowPadding = ImVec2(0, 0);
	style.PopupRounding = 0.0f;
	style.FramePadding = ImVec2(0, 0);
	style.ItemSpacing = ImVec2(0, 0);
	style.ItemInnerSpacing = ImVec2(0, 0);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 0.0f;
	style.ScrollbarSize = 0.0f;
	style.GrabMinSize = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.ChildBorderSize = 0.0f;
	style.PopupBorderSize = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.WindowRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0, 0);
	style.ButtonTextAlign = ImVec2(0, 0);

	/*
	@@NOTE

	We can either do hide the resize grip (this) so it doesn't show
	in he lower left corner or implement a custom ImGui function that
	internally behaves like a window but also shows resize grips on all sides

	*/
	style.Colors[ImGuiCol_ResizeGrip].w = 0.0f;

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

			m_new_collider.width = sprite_mouse_position.x - m_new_collider.x;
			m_new_collider.height = sprite_mouse_position.y - m_new_collider.y;

		}
		else if (m_dragging) { // We just finished collider creation

			m_currently_creating_collider = false;

			// We need to check if the size is not negative (the user dragged from down-right to up-left)
			{
				if (m_new_collider.width < 0) {
					m_new_collider.x += m_new_collider.width;
					m_new_collider.width *= -1;
				}

				if (m_new_collider.height < 0) {
					m_new_collider.y += m_new_collider.height;
					m_new_collider.height *= -1;
				}

			}

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

	auto draw_rects_on_top_of_sprite = true;

	const auto current_sprite_index = SpriteManager::get().get_current_main_sprite_index();

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
				draw_rects_on_top_of_sprite = true;

			}
			else if (m_current_tool == Tool::CREATE_COLLIDER) {
				ImGui::Text("Create Rect Tool");
				show_collider_selection = true;
				draw_rects_on_top_of_sprite = true;
			}
			else if (m_current_tool == Tool::EDIT_COLLIDER) {

				ImGui::Text("Edit Collider Tool");
				show_collider_selection = true;
				draw_rects_on_top_of_sprite = false;

				if (m_show_all_rects) {
					for (auto & pair : colliders) {
						const auto & type = pair.first;
						auto & instances = pair.second;
						for (auto & instance : instances) {
							edit_rects_of_instance(type, const_cast<ColliderInstance&>(instance), current_sprite_index);
						}
					}

				}
				else {

					if (m_p_selected_type &&  m_p_selected_instance) {
						edit_rects_of_instance(*m_p_selected_type, *m_p_selected_instance, current_sprite_index);
					}

				}

			}
			else {
				assert(false); // Unknown value for the selected tool
			}


			if (show_collider_selection) {


				//// Selecting Type for new rect
				MyImGui::SameLine();
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
					MyImGui::SameLine();
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


			MyImGui::SameLine();
			ImGui::Checkbox("Show All Rects", &m_show_all_rects);


			if (m_p_selected_type)
				m_selected_type_copy = *m_p_selected_type;

			if (m_p_selected_instance)
				m_selected_instance_copy = *m_p_selected_instance;


			MyImGui::SameLine();
			ImGui::Text("\t");


#pragma region modifying_tools_window

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
				auto drawing_collider_rect_size = GUIManager::get().sprite_to_global(sf::Vector2f(
					m_new_collider.x + m_new_collider.width,
					m_new_collider.y + m_new_collider.height));


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

	if (draw_rects_on_top_of_sprite) {
		if (ImGui::Begin("##HiddenFullscreen", nullptr,
			hidden_flags))
		{

			ImGui::BeginChild("##AvoidingLimit", ImVec2(0, 0), false, hidden_flags);

			// We draw the colliders in any position of the window
			auto draw_list = ImGui::GetWindowDrawList();

			draw_list->PushClipRectFullScreen();

			if (m_show_all_rects) {
				for (const auto & pair : colliders) {
					const auto & instances = pair.second;
					for (const auto & instance : instances) {
						draw_rects_of_instance(instance, current_sprite_index, &draw_list);
					}
				}

			}
			else {

				if (m_p_selected_instance) {

					draw_rects_of_instance(*m_p_selected_instance, current_sprite_index, &draw_list);

				}

			}


			draw_list->PopClipRect();


			ImGui::EndChild();
		}
		ImGui::End();
	}


	m_reread_rects = false;
}

void ToolsManager::reset_tools() {
	m_current_tool = Tool::NONE;
	m_p_selected_type = nullptr;
	m_p_selected_instance = nullptr;
	m_currently_creating_collider = false;
	m_dragging = false;
	m_show_all_rects = true;
}

void ToolsManager::draw_rects_of_instance(const ColliderInstance & instance, int current_sprite_index, ImDrawList ** draw_list) {

	auto border_color = ImColor(
		instance.color.x,
		instance.color.y,
		instance.color.z);

	auto fill_color = border_color;
	fill_color.Value.w = 0.4f;

	if (instance.rects.find(current_sprite_index) != instance.rects.end()) {
		for (const auto& rect : instance.rects.at(current_sprite_index)) {

			/*
			If we are drawing too many vertices we end the child and start another to push the
			current vertices to the screen
			*/
			if (sizeof(ImDrawIdx) == 2) {
				if (((*draw_list)->VtxBuffer.Size + INDEX_THRESHOLD_TO_ADD) > (1 << 16)) {
					ImGui::EndChild();
					ImGui::BeginChild("##AvoidingLimit", ImVec2(0, 0), false, hidden_flags);
					*draw_list = ImGui::GetWindowDrawList();
					(*draw_list)->PushClipRectFullScreen();
				}
			}
			else {
				assert(false); // We are not checking for larger draw indices sizes.
			}


			auto drawing_collider_rect_a = GUIManager::get().sprite_to_global(sf::Vector2f(rect.x, rect.y));
			auto drawing_collider_rect_b = GUIManager::get().sprite_to_global(sf::Vector2f(rect.x + rect.width, rect.y + rect.height));

			(*draw_list)->AddRectFilled(drawing_collider_rect_a, drawing_collider_rect_b,
				ImGui::GetColorU32((ImVec4)fill_color));

			(*draw_list)->AddRect(drawing_collider_rect_a, drawing_collider_rect_b,
				ImGui::GetColorU32((ImVec4)border_color));
		}
	}

}

void ToolsManager::edit_rects_of_instance(const ColliderType & type, ColliderInstance & instance, int current_sprite_index) {

	auto border_color = ImColor(
		instance.color.x,
		instance.color.y,
		instance.color.z);

	auto fill_color = border_color;
	fill_color.Value.w = 0.4f;

	auto focused_color = border_color;
	focused_color.Value.w = 0.6f;


	auto previous_style = ImGui::GetStyle();
	auto & style = ImGui::GetStyle();
	style = m_edit_collider_style;

	if (instance.rects.find(current_sprite_index) != instance.rects.end()) {
		auto rect_index = 0;

		/* @@TODO @@REMOVE
		auto index_to_delete = -1;
		*/

		auto& rects = instance.rects.at(current_sprite_index);

		for (auto& rect : rects) {

			auto collider_rect_origin = GUIManager::get().sprite_to_global(sf::Vector2f(rect.x, rect.y));
			auto collider_rect_size = GUIManager::get().sprite_to_global(sf::Vector2f(rect.x + rect.width, rect.y + rect.height));
			collider_rect_size -= collider_rect_origin;

			{
				const auto window_name = generate_edit_rect_name(type.name, instance.name, current_sprite_index, rect_index);

				ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)fill_color);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)focused_color);

				if (m_reread_rects) {
					ImGui::SetNextWindowPos(collider_rect_origin, ImGuiCond_Always);
					ImGui::SetNextWindowSize(collider_rect_size, ImGuiCond_Always);
				}
				else {
					ImGui::SetNextWindowPos(collider_rect_origin, ImGuiCond_Appearing);
					ImGui::SetNextWindowSize(collider_rect_size, ImGuiCond_Appearing);
				}

				ImGui::SetNextWindowSizeConstraints(ImVec2(0.f, 0.f), ImVec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));

				ImGui::Begin(window_name.c_str(), nullptr,
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_ResizeFromAnySide |
					ImGuiWindowFlags_NoScrollbar
				);
				{

					/*
					@@TODO

					Add here a way to delete the rect. I'm thinking
					that we could use a right click to pop a menu so we can put other options
					in the future :)


					@@TODO

					Also, maybe add here a way to copy the rect to a range of frames?
					*/

					auto window_pos_a = ImGui::GetWindowPos();
					auto window_size = ImGui::GetWindowSize();
					auto window_pos_b = window_pos_a + window_size;

					if (!m_reread_rects) {

						auto pos_a = GUIManager::get().global_to_sprite(window_pos_a);
						auto pos_b = GUIManager::get().global_to_sprite(window_pos_b);
						auto size = pos_b - pos_a;

						rect.x = pos_a.x;
						rect.y = pos_a.y;
						rect.width = size.x;
						rect.height = size.y;
					}

					if (ImGui::IsWindowFocused()) {
						auto draw_list = ImGui::GetWindowDrawList();
						draw_list->PushClipRectFullScreen();
						draw_list->AddRect(window_pos_a, window_pos_b,
							ImGui::GetColorU32((ImVec4)border_color));

						ColliderManager::get().set_selected_window_name(window_name);
					}

					{
						style = previous_style;

						if (ImGui::BeginPopupContextWindow()) {

							if (ImGui::MenuItem("Delete Rect")) {
								/*
								@@TODO
								
								Call the collider manager and tell it to delete a
								particular rect in a particular type, instance and sprite.

								We can't do it here because then the position will be set wrongly
								if we are setting the position with the Inputs in the editor.

								Just delete the rect in the .tick or .update of the collider editor
								and call .reread_rect_positions() so this doesn't set the colliders
								that remain to the old positions that the one with their index had.
								*/
								assert(false); // Read above you mad man.
							}
							ImGui::EndPopup();
						}
						
						style = m_edit_collider_style;
					}


				}
				ImGui::End();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}

			++rect_index;
		}

		/* @@TODO @@REMOVE
		if (index_to_delete >= 0 && index_to_delete < rects.size()) {
			CLOG("Deleting Rect (" << index_to_delete << "): " << &rects[index_to_delete]);
			rects.erase(rects.begin() + index_to_delete);
			index_to_delete = -1;
			m_reread_rects = true;
		}
		*/


	}
	style = previous_style;
}
