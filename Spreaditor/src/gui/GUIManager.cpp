#include "GUIManager.h"
#include "../time/TimeManager.h"
#include <Debug.h>
#include <../config/Config.h>
#include "../debug/cafe.inl"
#include "../utils/File.h"
#include "../render/RenderManager.h"
#include "gsl\gsl"
#include "../os/OSManager.h"
#include "GUIUtils.h"
#include "../sprites/SpriteManager.h"
#include "../sprites/Spritesheet.h"
#include "../colliders/ColliderManager.h"
#include "../tools/ToolsManager.h"


GUIManager::GUIManager() {}

GUIManager::~GUIManager() {}

void GUIManager::start_up() {

	if (file_exists(config::style_filename)) {
		CLOG("Loading style from file: " << config::style_filename);
		auto file_content = read_from_file(config::style_filename);

		auto json_data = json::parse(file_content, nullptr, false);

		if (json_data.is_discarded()) {
			CLOG("\tThe style file is not a valid file, ignoring style loading.");

			// @@TODO: Load here a default style that is generally usable and we are happy with

		}
		else {
			style_from_json(json_data);
		}
	}

	// We are not using this font right now
	/*
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	m_font = io.Fonts->AddFontFromMemoryCompressedTTF(cafe_compressed_data, cafe_compressed_size, 18.0f);
	ImGui::SFML::UpdateFontTexture();
	*/

	ImGui::GetIO().MouseDrawCursor = true;

}

void GUIManager::shut_down() {


}

void GUIManager::set_debug_open(bool open) {
	m_debug_open = open;
}

bool GUIManager::is_debug_open() const {
	return m_debug_open;
}

const sf::Vector2f & GUIManager::sprite_to_global(sf::Vector2f sprite_pos) const {

	const auto& main_sprite = SpriteManager::get().get_cached_drawn_main_sprite();
	const auto& sprite_bounds = main_sprite.getGlobalBounds();

	return sf::Vector2f(
		((sprite_pos.x / main_sprite.getTextureRect().width) * sprite_bounds.width) + sprite_bounds.left,
		((sprite_pos.y / main_sprite.getTextureRect().height) * sprite_bounds.height) + sprite_bounds.top
	);

}

const sf::Vector2f & GUIManager::global_to_sprite(sf::Vector2f global_pos) const {

	const auto& main_sprite = SpriteManager::get().get_cached_drawn_main_sprite();
	const auto& sprite_bounds = main_sprite.getGlobalBounds();

	return sf::Vector2f(
		((global_pos.x - sprite_bounds.left) / sprite_bounds.width)* main_sprite.getTextureRect().width,
		((global_pos.y - sprite_bounds.top) / sprite_bounds.height)* main_sprite.getTextureRect().height
	);

}

void GUIManager::update() {

	// Mouse Updates
	{
		auto mouse_pos = ImGui::GetIO().MousePos;

		m_global_mouse_pos = RenderManager::get().get_main_render_target()->mapPixelToCoords(sf::Vector2i(
			gsl::narrow_cast<int>(mouse_pos.x),
			gsl::narrow_cast<int>(mouse_pos.y)));

		const auto& main_sprite = SpriteManager::get().get_cached_drawn_main_sprite();
		const auto& sprite_bounds = main_sprite.getGlobalBounds();
		m_is_mouse_inside_sprite = sprite_bounds.contains(m_global_mouse_pos);

		m_sprite_mouse_position = global_to_sprite(mouse_pos);


		if (ImGui::GetIO().MouseDrawCursor)
			SetCursor(NULL);
	}

	// Scaling
	{
		ImGui::GetIO().FontGlobalScale = m_scaling_factor;
		ImGui::SetSamelineScaling(m_scaling_factor);
		// @@TODO: Scale also the places where I set pixels exactly like in the samelines
	}


}

void GUIManager::do_gui() {

	ImGui::PushFont(m_font);

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {

		BEGIN_MENU_POPUP_MODAL("New Project From Spritesheet");
		{

			static auto filename = std::string{};

			static char loading_filename[MAX_OS_FILENAME_SIZE];

			ImGui::Text("Filename: ");
			ImGui::SameLine();
			ImGui::InputText("##Filename", loading_filename, MAX_OS_FILENAME_SIZE);
			ImGui::SameLine();
			if (ImGui::Button("Explore", ImVec2(120, 0))) {
				filename = OSManager::get().user_open_file(\
					"(*.png) Portable Network Graphics\0*.png\0"
					"(*.bmp) Windows bitmap\0*.bmp\0"
					"(*.jpg) Joint Photographic Experts Group\0*.jpg\0"

				);
				assert(filename.size() < MAX_OS_FILENAME_SIZE);
				filename.copy(loading_filename, filename.size());
				loading_filename[filename.size()] = '\0';
			}

			ImGui::TextWrapped("This will close the current project without keeping unsaved changes.");


			IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
			{
				ImGui::CloseCurrentPopup();
			}
			END_BUTTON_ALIGNED_RIGHT_FIRST;

			IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

				filename.clear();
				filename.append(loading_filename);

				CLOG("Loading spritesheet from " << filename);

				auto cfilename = filename.c_str();

				if (!file_exists(cfilename)) {
					CLOG_ERROR("Incorrect File Path");
					ImGui::OpenPopup("Incorrect File Path");
				}
				else {

					auto loaded = SpriteManager::get().load_spritesheet(cfilename);

					auto spritesheet = SpriteManager::get().get_spritesheet();

					if (!loaded || !spritesheet) {
						CLOG_ERROR("Incorrect Spritesheet");
						ImGui::OpenPopup("Incorrect Spritesheet");
					}
					else {

						CLOG("\tSpritesheet rows: " << spritesheet->get_rows());
						CLOG("\tSpritesheet columns: " << spritesheet->get_cols());
						CLOG("\tSprite width: " << spritesheet->get_sprite_width());
						CLOG("\tSprite height: " << spritesheet->get_sprite_height());
						CLOG("\tAmount of sprites read: " << spritesheet->get_sprites().size());

						CLOG("Correctly loaded spritesheet from " << filename);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			END_BUTTON_ALIGNED_RIGHT_NEXT(accept);

			if (ImGui::BeginPopupModal("Incorrect File Path"))
			{
				ImGui::Text("The spritesheet file you are trying to open doesn't exist:\nPath: \"%s\"", filename.c_str());

				IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))
				{
					ImGui::CloseCurrentPopup();
				}
				END_BUTTON_ALIGNED_RIGHT_FIRST;

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("Incorrect Spritesheet"))
			{
				ImGui::Text("The spritesheet file could not be parsed correctly:\nPath: \"%s\"", filename.c_str());

				IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))
				{
					ImGui::CloseCurrentPopup();
				}
				END_BUTTON_ALIGNED_RIGHT_FIRST;

				ImGui::EndPopup();
			}

		}
		END_MENU_POPUP_MODAL;

		ImGui::Separator();


		BEGIN_MENU_POPUP_MODAL("Export Spritesheet");
		{
			static auto filename = std::string{};

			static char writing_filename[MAX_OS_FILENAME_SIZE];

			ImGui::Text("Filename: ");
			ImGui::SameLine();
			ImGui::InputText("##Filename", writing_filename, MAX_OS_FILENAME_SIZE);
			ImGui::SameLine();
			if (ImGui::Button("Explore", ImVec2(120, 0))) {
				filename = OSManager::get().user_save_file(\
					"(*.png) Portable Network Graphics\0*.png\0"
					"(*.bmp) Windows bitmap\0*.bmp\0"
					"(*.jpg) Joint Photographic Experts Group\0*.jpg\0"

				);
				assert(filename.size() < MAX_OS_FILENAME_SIZE);
				filename.copy(writing_filename, filename.size());
				writing_filename[filename.size()] = '\0';
			}


			// We check if the filename the user has written already exists
			static auto file_already_exists = false;
			static auto last_time_checked = .0f;
			auto current_time = TimeManager::get().get_execution_time().asMilliseconds();
			if (current_time - last_time_checked > CHECK_FILE_INTERVAL) {
				last_time_checked = current_time;
				file_already_exists = file_exists(writing_filename);
			}

			if (file_already_exists) ImGui::Text("WARNING: A File with that name already exists");


			IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
			{
				ImGui::CloseCurrentPopup();
			}
			END_BUTTON_ALIGNED_RIGHT_FIRST;

			IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

				filename.clear();
				filename.append(writing_filename);

				CLOG("Writing spritesheet to: " << filename);

				auto cfilename = filename.c_str();

				if (!SpriteManager::get().write_sprites_to_spritesheet(cfilename)) {
					CLOG("An error has happened while writing the spritesheet");
					ImGui::OpenPopup("Error Writing Spritesheet");
				}
				else {
					CLOG("Spritesheet write completed succesfully to file: " << filename);
					ImGui::CloseCurrentPopup();
				}


			}
			END_BUTTON_ALIGNED_RIGHT_NEXT(accept);

			if (ImGui::BeginPopupModal("Error Writing Spritesheet"))
			{
				ImGui::Text("The spritesheet file could not be parsed correctly:\nPath: \"%s\"", filename.c_str());

				IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))
				{
					ImGui::CloseCurrentPopup();
				}
				END_BUTTON_ALIGNED_RIGHT_FIRST;

				ImGui::EndPopup();
			}
		}
		END_MENU_POPUP_MODAL;

		BEGIN_MENU_POPUP_MODAL("Export Collider File");
		{
			static auto filename = std::string{};

			static char writing_filename[MAX_OS_FILENAME_SIZE];

			ImGui::Text("Filename: ");
			ImGui::SameLine();
			ImGui::InputText("##Filename", writing_filename, MAX_OS_FILENAME_SIZE);
			ImGui::SameLine();
			if (ImGui::Button("Explore", ImVec2(120, 0))) {
				filename = OSManager::get().user_save_file(\
					"(*.json) JavaScript Object Notation\0*.json\0"
				);
				assert(filename.size() < MAX_OS_FILENAME_SIZE);
				filename.copy(writing_filename, filename.size());
				writing_filename[filename.size()] = '\0';
			}


			// We check if the filename the user has written already exists
			static auto file_already_exists = false;
			static auto last_time_checked = .0f;
			auto current_time = TimeManager::get().get_execution_time().asMilliseconds();
			if (current_time - last_time_checked > CHECK_FILE_INTERVAL) {
				last_time_checked = current_time;
				file_already_exists = file_exists(writing_filename);
			}

			if (file_already_exists) ImGui::Text("WARNING: A File with that name already exists");


			IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
			{
				ImGui::CloseCurrentPopup();
			}
			END_BUTTON_ALIGNED_RIGHT_FIRST;

			IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

				filename.clear();
				filename.append(writing_filename);

				CLOG("Writing colliders to: " << filename);

				auto cfilename = filename.c_str();

				if (!ColliderManager::get().write_colliders_to_file(cfilename)) {
					CLOG("An error has happened while writing the colliders");
					ImGui::OpenPopup("Error Writing Colliders");
				}
				else {
					CLOG("Colliders write completed succesfully to file: " << filename);
					ImGui::CloseCurrentPopup();
				}


			}
			END_BUTTON_ALIGNED_RIGHT_NEXT(accept);

			if (ImGui::BeginPopupModal("Error Writing Colliders"))
			{
				ImGui::Text("The collider file could not be written correctly:\nPath: \"%s\"", filename.c_str());

				IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))
				{
					ImGui::CloseCurrentPopup();
				}
				END_BUTTON_ALIGNED_RIGHT_FIRST;

				ImGui::EndPopup();
			}
		}
		END_MENU_POPUP_MODAL;


		// ====== END OF FILE MENU ======
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		ImGui::Checkbox("Timeline", &m_show_timeline);
		ImGui::Checkbox("Tools", &m_show_tools);
		ImGui::Checkbox("Collider Explorer", &m_show_collider_explorer);
		ImGui::Separator();
		ImGui::Checkbox("Style Editor", &m_show_style_editor);
		{
			if (ImGui::BeginMenu("Scaling")) {
				if (ImGui::MenuItem("100%", NULL, m_scaling_factor == 1.00f)) m_scaling_factor = 1.0f;
				if (ImGui::MenuItem("125%", NULL, m_scaling_factor == 1.25f)) m_scaling_factor = 1.25f;
				if (ImGui::MenuItem("150%", NULL, m_scaling_factor == 1.50f)) m_scaling_factor = 1.50f;
				if (ImGui::MenuItem("175%", NULL, m_scaling_factor == 1.75f)) m_scaling_factor = 1.75f;
				if (ImGui::MenuItem("200%", NULL, m_scaling_factor == 2.00f)) m_scaling_factor = 2.00f;
				if (ImGui::MenuItem("225%", NULL, m_scaling_factor == 2.25f)) m_scaling_factor = 2.25f;
				if (ImGui::MenuItem("250%", NULL, m_scaling_factor == 2.50f)) m_scaling_factor = 2.50f;
				if (ImGui::MenuItem("300%", NULL, m_scaling_factor == 3.00f)) m_scaling_factor = 3.00f;
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Debug")) {
		ImGui::Checkbox("Debug Overlay", &m_show_debug_overlay);
		ImGui::Checkbox("ImGui Demo", &m_show_imgui_demo);
		ImGui::EndMenu();
	}

	m_main_menu_height = ImGui::GetWindowSize().y;

	ImGui::EndMainMenuBar();

	if (m_show_imgui_demo) ImGui::ShowDemoWindow();
	if (m_show_timeline) draw_timeline();
	if (m_show_tools) ToolsManager::get().draw_tools_gui();
	if (m_show_style_editor) draw_style_editor();
	if (m_show_debug_overlay) draw_corner_overlay_debug_info();
	if (m_show_collider_explorer) ColliderManager::get().draw_collider_gui();


	ImGui::PopFont();
}

void GUIManager::draw_corner_overlay_debug_info() {
	bool open = true;
	const float DISTANCE = 10.0f;
	static int corner = 0;
	auto extra_vertical_margin = get_main_menu_height() + ToolsManager::get().tools_options_bar_height();
	auto window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE - ColliderManager::get().timeline_width() : DISTANCE + ToolsManager::get().tools_width(), (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE - m_timeline_height : (DISTANCE + extra_vertical_margin));
	auto window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	auto background = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
	background.w = 0.5f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background); // Transparent background
	if (ImGui::Begin("Debug Overlay", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::Text("Mouse Position (Global): (%6.1f,%6.1f)", m_global_mouse_pos.x, m_global_mouse_pos.y);

		if (m_is_mouse_inside_sprite) {

			ImGui::Text("Mouse Position (Sprite): (%6.1f,%6.1f)", m_sprite_mouse_position.x, m_sprite_mouse_position.y);
		}
		else {
			ImGui::Text("Mouse not on top of main sprite");
		}

		ImGui::Text("Frames Per Second: (%.1f)", (1.f / TimeManager::get().get_delta_time().asSeconds()));
		ImGui::Text("Frame Time: (%d) ms", (TimeManager::get().get_delta_time().asMilliseconds()));
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
	ImGui::PopStyleColor();
}

void GUIManager::draw_timeline() {
	auto screen_size = RenderManager::get().get_main_render_target()->getSize();
	ImGui::SetNextWindowSize(ImVec2(screen_size.x, 0));
	
	if (ImGui::Begin("Timeline", nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_MenuBar
	)) {


		ImGui::BeginMenuBar();
		{
			auto main_sprite_index_cached = SpriteManager::get().get_current_main_sprite_index();

			auto sprite_count = SpriteManager::get().get_sprites().size();
			sprite_count = sprite_count > 0 ? sprite_count : 0;

			auto show_sprite_options = sprite_count > 0;

			ImGui::Text("Timeline");

			if (show_sprite_options) {
				ImGui::Text("\t");
				ImGui::SliderInt("##Selected Sprite", &main_sprite_index_cached, 0, sprite_count - 1, "Sprite [%.0f]");

				auto button_size = ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight());

				if (ImGui::Button("<##GoBackOneSprite", button_size)) {
					if (main_sprite_index_cached > 0) {
						--main_sprite_index_cached;
					}
				}

				/*
				Use this in case we want to allow the user to directly write the sprite he wants to go to
				The formatting of the input is a bit weird (too wide) right now, so that would need to be fixed first
				*/
				//ImGui::InputInt("##InputTextSpriteIndex", &main_sprite_index_cached, 1, 20, ImGuiInputTextFlags_NoEditButtons);

				if (ImGui::Button(">##GoForwardOneSprite", button_size)) {
					if (main_sprite_index_cached < sprite_count - 1) {
						++main_sprite_index_cached;
					}
				}

				// Clamp the value to [0-max sprite index]
				main_sprite_index_cached = main_sprite_index_cached > sprite_count - 1 ? sprite_count - 1 : main_sprite_index_cached;
				main_sprite_index_cached = main_sprite_index_cached < 0 ? 0 : main_sprite_index_cached;

				SpriteManager::get().set_current_main_sprite_index(main_sprite_index_cached);
			}
			else {
				ImGui::Text("\tNo sprites available");
			}



		}
		ImGui::EndMenuBar();


		ImGui::BeginChild("##TimelineScrollingRegion", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * TIMELINE_SIZE), false, ImGuiWindowFlags_HorizontalScrollbar);
		{
			auto sprites = SpriteManager::get().get_sprites();
			auto sprite_count = sprites.size();
			auto timeline_sprite_height = ImGui::GetTextLineHeightWithSpacing() * (TIMELINE_SIZE - TIMELINE_HEADER_SIZE) - ImGui::GetStyle().ScrollbarSize;
			if (sprite_count > 0) {

				draw_timeline_sprite(sprites[0], 0, timeline_sprite_height);

				for (int i = 1; i < sprite_count; i++)
				{
					ImGui::SameLine();
					draw_timeline_sprite(sprites[i], i, timeline_sprite_height);

				}
			}
		}
		ImGui::EndChild();

		auto window_height = ImGui::GetCursorPos().y;
		ImGui::SetWindowPos(ImVec2(0, screen_size.y - window_height), true);
		m_timeline_height = window_height;
	}
	ImGui::End();

}

void GUIManager::draw_timeline_sprite(const sf::Sprite& sprite, int sprite_index, float height) {

	auto temp_sprite = sprite;
	temp_sprite.setScale(
		(height) / static_cast<float>(temp_sprite.getTextureRect().height),
		(height) / static_cast<float>(temp_sprite.getTextureRect().height));
	temp_sprite.setPosition(sf::Vector2f(0.f, 0.f));
	auto sprite_id = std::string("sprite_");
	sprite_id += sprite_index;
	ImGui::PushID(sprite_id.c_str());


	auto previous_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPos(previous_pos + ImGui::GetStyle().FramePadding);
	ImGui::Text("%d", sprite_index);
	ImGui::SetCursorPos(previous_pos);

	auto is_main_sprite = sprite_index == SpriteManager::get().get_current_main_sprite_index();

	if (is_main_sprite) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
	}

	if (ImGui::ImageButton(temp_sprite)) {
		SpriteManager::get().set_current_main_sprite_index(sprite_index);
	}

	if (is_main_sprite) {
		ImGui::PopStyleColor();
	}

	ImGui::PopID();
}

void GUIManager::draw_style_editor(ImGuiStyle* ref) {


	ImGui::Begin("Style Editor");

	// You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to an internally stored reference)
	ImGuiStyle& style = ImGui::GetStyle();
	static ImGuiStyle ref_saved_style;

	// Default to using internal storage as reference
	static bool init = true;
	if (init && ref == NULL)
		ref_saved_style = style;
	init = false;
	if (ref == NULL)
		ref = &ref_saved_style;

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

	if (ImGui::ShowStyleSelector("Color Themes##Selector")) {
		ref_saved_style = style;
	}

	//We only allow one font for now
	//ImGui::ShowFontSelector("Fonts##Selector");

	// Simplified Settings
	if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
		style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	{ bool window_border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &window_border)) style.WindowBorderSize = window_border ? 1.0f : 0.0f; }
	ImGui::SameLine();
	{ bool frame_border = (style.FrameBorderSize > 0.0f); if (ImGui::Checkbox("FrameBorder", &frame_border)) style.FrameBorderSize = frame_border ? 1.0f : 0.0f; }
	ImGui::SameLine();
	{ bool popup_border = (style.PopupBorderSize > 0.0f); if (ImGui::Checkbox("PopupBorder", &popup_border)) style.PopupBorderSize = popup_border ? 1.0f : 0.0f; }

	// Save/Revert button
	if (ImGui::Button("Save Local"))
		*ref = ref_saved_style = style;
	ImGui::SameLine();
	if (ImGui::Button("Revert Local"))
		style = *ref;
	ImGui::SameLine();
	show_help_marker("This saves the local style for the current execution of the program.\n\nThe style saved with this won't be maintained between executions "
		"so if you close the program and open it again the style saved with this options will be lost.\n\n"
		"This is meant to save temporarily a style that you like and keep changing it while giving you the option to revert to the saved one quickly if you so desire");

	// Save To File
	if (ImGui::Button("Save To File")) {

		auto json_style = style_to_json();

		CLOG("Saving style to json file: " << config::style_filename);

		write_to_file(config::style_filename, json_style.dump().c_str());

	}
	ImGui::SameLine();
	show_help_marker("This saves the local style to a file that will be used when opening the program so the style can be maintained when opening"
		" and closing the program again.");

	if (ImGui::TreeNode("Settings"))
	{
		ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
		ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
		ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
		ImGui::Text("BorderSize");
		ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::Text("Rounding");
		ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 14.0f, "%.0f");
		ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
		ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
		ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
		ImGui::Text("Alignment");
		ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); show_help_marker("Alignment applies when a button is larger than its text content.");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Colors"))
	{

		ImGui::Text("Tip: Left-click on colored square to open color picker,\nRight-click to open edit options menu.");

		static ImGuiTextFilter filter;
		filter.Draw("Filter colors", 200);

		static ImGuiColorEditFlags alpha_flags = 0;
		ImGui::RadioButton("Opaque", &alpha_flags, 0); ImGui::SameLine();
		ImGui::RadioButton("Alpha", &alpha_flags, ImGuiColorEditFlags_AlphaPreview); ImGui::SameLine();
		ImGui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf);

		ImGui::BeginChild("#colors", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		ImGui::PushItemWidth(-160);
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName(i);
			if (!filter.PassFilter(name))
				continue;
			ImGui::PushID(i);
			ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
			if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
			{
				// Tips: in a real user application, you may want to merge and use an icon font into the main font, so instead of "Save"/"Revert" you'd use icons.
				// Read the FAQ and extra_fonts/README.txt about using icon fonts. It's really easy and super convenient!
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i];
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) style.Colors[i] = ref->Colors[i];
			}
			ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
			ImGui::TextUnformatted(name);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();

		ImGui::TreePop();
	}

	ImGui::PopItemWidth();

	ImGui::End();
}

// All variables to save
/*

WindowPadding
WindowRounding
FramePadding
FrameRounding
ItemSpacing
ItemInnerSpacing
IndentSpacing
ScrollbarSize
ScrollbarRounding
GrabMinSize
GrabMinSize
GrabRounding
Colors[ImGuiCol_Text]
Colors[ImGuiCol_TextDisabled]
Colors[ImGuiCol_WindowBg]
Colors[ImGuiCol_ChildWindowBg]
Colors[ImGuiCol_PopupBg]
Colors[ImGuiCol_Border]
Colors[ImGuiCol_BorderShadow]
Colors[ImGuiCol_FrameBg]
Colors[ImGuiCol_FrameBgHovered]
Colors[ImGuiCol_FrameBgActive]
Colors[ImGuiCol_TitleBg]
Colors[ImGuiCol_TitleBgCollapsed]
Colors[ImGuiCol_TitleBgActive]
Colors[ImGuiCol_MenuBarBg]
Colors[ImGuiCol_ScrollbarBg]
Colors[ImGuiCol_ScrollbarGrab]
Colors[ImGuiCol_ScrollbarGrabHovered]
Colors[ImGuiCol_ScrollbarGrabActive]
Colors[ImGuiCol_CheckMark]
Colors[ImGuiCol_SliderGrab]
Colors[ImGuiCol_SliderGrabActive]
Colors[ImGuiCol_Button]
Colors[ImGuiCol_ButtonHovered]
Colors[ImGuiCol_ButtonActive]
Colors[ImGuiCol_Header]
Colors[ImGuiCol_HeaderHovered]
Colors[ImGuiCol_HeaderActive]
Colors[ImGuiCol_Column]
Colors[ImGuiCol_ColumnHovered]
Colors[ImGuiCol_ColumnActive]
Colors[ImGuiCol_ResizeGrip]
Colors[ImGuiCol_ResizeGripHovered]
Colors[ImGuiCol_ResizeGripActive]
Colors[ImGuiCol_CloseButton]
Colors[ImGuiCol_CloseButtonHovered]
Colors[ImGuiCol_CloseButtonActive]
Colors[ImGuiCol_PlotLines]
Colors[ImGuiCol_PlotLinesHovered]
Colors[ImGuiCol_PlotHistogram]
Colors[ImGuiCol_PlotHistogramHovered]
Colors[ImGuiCol_TextSelectedBg]
Colors[ImGuiCol_ModalWindowDarkening]

*/

#define SAVE_STYLE_VAR(x) data[#x] = style.##x
#define LOAD_STYLE_VAR(x) style.##x = data[#x]

GUIManager::json GUIManager::style_to_json() {
	auto data = json{};
	auto style = ImGui::GetStyle();

	{
		SAVE_STYLE_VAR(WindowPadding);
		SAVE_STYLE_VAR(WindowRounding);
		SAVE_STYLE_VAR(FramePadding);
		SAVE_STYLE_VAR(FrameRounding);
		SAVE_STYLE_VAR(ItemSpacing);
		SAVE_STYLE_VAR(ItemInnerSpacing);
		SAVE_STYLE_VAR(IndentSpacing);
		SAVE_STYLE_VAR(ScrollbarSize);
		SAVE_STYLE_VAR(ScrollbarRounding);
		SAVE_STYLE_VAR(GrabMinSize);
		SAVE_STYLE_VAR(GrabMinSize);
		SAVE_STYLE_VAR(GrabRounding);
		SAVE_STYLE_VAR(Colors[ImGuiCol_Text]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_TextDisabled]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_WindowBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ChildWindowBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_PopupBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_Border]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_BorderShadow]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_FrameBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_FrameBgHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_FrameBgActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_TitleBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_TitleBgCollapsed]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_TitleBgActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_MenuBarBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrab]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_CheckMark]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_SliderGrab]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_SliderGrabActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_Button]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ButtonHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ButtonActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_Header]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_HeaderHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_HeaderActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_Column]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ColumnHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ColumnActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ResizeGrip]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ResizeGripHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ResizeGripActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_CloseButton]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_CloseButtonHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_CloseButtonActive]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_PlotLines]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_PlotLinesHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_PlotHistogram]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_PlotHistogramHovered]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_TextSelectedBg]);
		SAVE_STYLE_VAR(Colors[ImGuiCol_ModalWindowDarkening]);
	}

	return data;
}

void GUIManager::style_from_json(json data) {

	auto& style = ImGui::GetStyle();

	{
		LOAD_STYLE_VAR(WindowPadding);
		LOAD_STYLE_VAR(WindowRounding);
		LOAD_STYLE_VAR(FramePadding);
		LOAD_STYLE_VAR(FrameRounding);
		LOAD_STYLE_VAR(ItemSpacing);
		LOAD_STYLE_VAR(ItemInnerSpacing);
		LOAD_STYLE_VAR(IndentSpacing);
		LOAD_STYLE_VAR(ScrollbarSize);
		LOAD_STYLE_VAR(ScrollbarRounding);
		LOAD_STYLE_VAR(GrabMinSize);
		LOAD_STYLE_VAR(GrabMinSize);
		LOAD_STYLE_VAR(GrabRounding);
		LOAD_STYLE_VAR(Colors[ImGuiCol_Text]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_TextDisabled]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_WindowBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ChildWindowBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_PopupBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_Border]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_BorderShadow]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_FrameBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_FrameBgHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_FrameBgActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_TitleBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_TitleBgCollapsed]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_TitleBgActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_MenuBarBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrab]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_CheckMark]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_SliderGrab]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_SliderGrabActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_Button]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ButtonHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ButtonActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_Header]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_HeaderHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_HeaderActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_Column]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ColumnHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ColumnActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ResizeGrip]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ResizeGripHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ResizeGripActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_CloseButton]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_CloseButtonHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_CloseButtonActive]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_PlotLines]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_PlotLinesHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_PlotHistogram]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_PlotHistogramHovered]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_TextSelectedBg]);
		LOAD_STYLE_VAR(Colors[ImGuiCol_ModalWindowDarkening]);
	}

}
