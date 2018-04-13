#pragma once

#include <Manager.h>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Spritesheet.h"

class SpriteManager : public Manager<SpriteManager> {
    friend class CRSP<SpriteManager>;
private:
    SpriteManager();
    ~SpriteManager();

public:

    void start_up() override;
    void shut_down() override;


    bool load_spritesheet(const char* filename);

    Spritesheet* get_spritesheet() const { return m_spritesheet.get(); }
    float get_main_sprite_zoom() const { return m_main_sprite_zoom; }
    void set_main_sprite_zoom(float zoom) { m_main_sprite_zoom = zoom; }
    int get_current_main_sprite_index() const { return m_current_main_sprite_index; }
    void set_current_main_sprite_index(int index) { m_current_main_sprite_index = index; }
    const std::vector<sf::Sprite>& get_sprites() { return m_sprites; }

    void render_main_sprite(sf::RenderTarget* render_target);

    bool write_sprites_to_spritesheet(const char* spritesheet_filename) const;

private:
    std::unique_ptr<Spritesheet> m_spritesheet{};
    std::vector<sf::Sprite> m_sprites{};
    float m_main_sprite_zoom{ 8.0f };
    int m_current_main_sprite_index{ 0 };
    bool m_valid_sprites{ false };
};