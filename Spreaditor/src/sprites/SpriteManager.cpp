#include "SpriteManager.h"
#include "../window/WindowManager.h"
#include "../gui/GUIManager.h"
#include "../math/OtherMath.h"

SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

void SpriteManager::start_up() {}

void SpriteManager::shut_down() {}

bool SpriteManager::load_spritesheet(const char * filename) {
    m_spritesheet = std::make_unique<Spritesheet>(filename);
    m_sprites = m_spritesheet->get_sprites();
    m_valid_sprites = m_spritesheet->is_valid();
    return m_valid_sprites;
}

bool SpriteManager::load_sprites(const std::vector<std::string>& filenames){
	m_spritesheet = std::make_unique<Spritesheet>(filenames);
	m_sprites = m_spritesheet->get_sprites();
	m_valid_sprites = m_spritesheet->is_valid();
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

bool SpriteManager::write_sprites_to_spritesheet(const char * spritesheet_filename) const {
	return m_spritesheet->write_to_file(spritesheet_filename, m_spritesheet->get_sprites());
}
