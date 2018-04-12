#include "SpriteManager.h"
#include "../window/WindowManager.h"
#include "../gui/GUIManager.h"

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

void SpriteManager::render_main_sprite(sf::RenderTarget* render_target) {

    if (!m_valid_sprites ||  m_current_main_sprite_index >= m_sprites.size()) return;

    auto main_sprite = m_sprites[m_current_main_sprite_index];

    main_sprite.setOrigin(main_sprite.getTextureRect().width/2.0f, main_sprite.getTextureRect().height / 2.0f);
    main_sprite.setScale(m_main_sprite_zoom, m_main_sprite_zoom);

    auto timeline_offset = (TIMELINE_SIZE * ImGui::GetTextLineHeightWithSpacing()) / 2.f;
    auto main_bar_offset = ImGui::GetTextLineHeightWithSpacing();

    main_sprite.setPosition(render_target->getView().getSize().x / 2.f, render_target->getView().getSize().y / 2.f - timeline_offset + main_bar_offset);

    render_target->draw(main_sprite);
}
