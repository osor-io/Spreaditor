#include "SpriteManager.h"



SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

void SpriteManager::start_up() {}

void SpriteManager::shut_down() {}

bool SpriteManager::load_spritesheet(const char * filename) {
    m_spritesheet = std::make_unique<Spritesheet>(filename);
    m_sprites = m_spritesheet->get_sprites();
    return m_spritesheet->is_valid();
}

void SpriteManager::render_main_sprite(sf::RenderTarget* render_target) {

    if (m_current_main_sprite_index >= m_sprites.size()) return;

    auto main_sprite = m_sprites[m_current_main_sprite_index];

    main_sprite.setOrigin(main_sprite.getTextureRect().width/2.0f, main_sprite.getTextureRect().height / 2.0f);
    main_sprite.setPosition(render_target->getSize().x / 2.f, render_target->getSize().y / 2.f);
    main_sprite.setScale(m_main_sprite_zoom, m_main_sprite_zoom);

    render_target->draw(main_sprite);

    //@@TODO @@DEBUG @@REMOVE

    ++m_current_main_sprite_index;
    m_current_main_sprite_index %= m_sprites.size();

}
