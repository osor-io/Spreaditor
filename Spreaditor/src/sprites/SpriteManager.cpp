#include "SpriteManager.h"
#include "../window/WindowManager.h"
#include "../gui/GUIManager.h"
#include "../tools/ToolsManager.h"
#include "../render/RenderManager.h"
#include "../math/OtherMath.h"

SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

void SpriteManager::start_up() {}

void SpriteManager::shut_down() {}

bool SpriteManager::load_spritesheet(const char * filename) {
	m_spritesheet = std::make_unique<Spritesheet>(filename);
	m_sprites = m_spritesheet->get_sprites();
	m_valid_sprites = m_spritesheet->is_valid();
	set_default_zoom();
	return m_valid_sprites;
}

bool SpriteManager::load_sprites(const std::vector<std::string>& filenames) {
	m_spritesheet = std::make_unique<Spritesheet>(filenames);
	m_sprites = m_spritesheet->get_sprites();
	m_valid_sprites = m_spritesheet->is_valid();
	set_default_zoom();
	return m_valid_sprites;
}


void SpriteManager::render_main_sprite(sf::RenderTarget* render_target) {

	if (!m_valid_sprites || m_current_main_sprite_index >= m_sprites.size()) return;

	m_drawn_main_sprite_cached = m_sprites[m_current_main_sprite_index];

	m_drawn_main_sprite_cached.setOrigin(m_drawn_main_sprite_cached.getTextureRect().width / 2.0f, m_drawn_main_sprite_cached.getTextureRect().height / 2.0f);
	m_drawn_main_sprite_cached.setScale(m_main_sprite_zoom, m_main_sprite_zoom);

	auto timeline_offset = (TIMELINE_SIZE * ImGui::GetTextLineHeightWithSpacing()) / 2.f;
	auto main_bar_offset = ImGui::GetTextLineHeightWithSpacing();

	m_drawn_main_sprite_cached.setPosition(render_target->getView().getSize().x / 2.f, render_target->getView().getSize().y / 2.f - timeline_offset + main_bar_offset);

	render_target->draw(m_drawn_main_sprite_cached);
}

json SpriteManager::spritesheet_to_json() const {
	assert(m_spritesheet);
	return m_spritesheet->to_json();
}

bool SpriteManager::write_sprites_to_spritesheet(const char * spritesheet_filename) const {
	return m_spritesheet->write_to_file(spritesheet_filename, m_spritesheet->get_sprites());
}

void SpriteManager::set_default_zoom() {

	//As wide as half of the screen and as tall as the size available minus something. Pick the lesser of the two
	assert(m_spritesheet);

	auto real_sprite_width = m_spritesheet->get_sprite_width();
	auto real_sprite_height = m_spritesheet->get_sprite_height();

	auto vertical_space =
		ImGui::GetIO().DisplaySize.y
		- GUIManager::get().get_timeline_height()
		- GUIManager::get().get_main_menu_height()
		- ToolsManager::get().tools_options_bar_height();

	auto horizontal_space = ImGui::GetIO().DisplaySize.y * 0.7f;

	auto desired_zoom_x = horizontal_space / real_sprite_width;
	auto desired_zoom_y = vertical_space / real_sprite_height;

	m_main_sprite_zoom = desired_zoom_x < desired_zoom_y ? desired_zoom_x : desired_zoom_y;
}
