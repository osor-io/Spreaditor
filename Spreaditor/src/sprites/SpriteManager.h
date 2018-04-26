#pragma once

#include <Manager.h>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Spritesheet.h"
#include <vector>
#include <string>

class SpriteManager : public Manager<SpriteManager> {
	friend class CRSP<SpriteManager>;
private:
	SpriteManager();
	~SpriteManager();

public:

	void start_up() override;
	void shut_down() override;


	bool load_spritesheet(const char* filename);
	bool load_spritesheet(const char* filename, int rows, int cols);
	bool load_spritesheet(const char* filename, int rows, int cols, int sprite_width, int sprite_height);
	bool load_sprites(const std::vector<std::string>& filenames);
	bool load_spritesheet_from_json(const std::string& filename, const json& data);
	json spritesheet_to_json() const;


	Spritesheet* get_spritesheet() const { return m_spritesheet.get(); }
	float get_main_sprite_zoom() const { return m_main_sprite_zoom; }
	void set_main_sprite_zoom(float zoom) { m_main_sprite_zoom = zoom; }
	int get_current_main_sprite_index() const { return m_playing_animation ? m_current_animation_sprite : m_current_main_sprite_index; }
	void set_current_main_sprite_index(int index) { m_current_main_sprite_index = index; }
	const std::vector<sf::Sprite>& get_sprites() { return m_sprites; }
	const sf::Sprite& get_cached_drawn_main_sprite() { return m_drawn_main_sprite_cached; }

	bool is_playing_animation() const { return m_playing_animation; }
	void toggle_play_animation() { m_playing_animation = !m_playing_animation; m_current_animation_sprite = m_first_animation_frame; }
	int& first_animation_frame() { return m_first_animation_frame; }
	int& last_animation_frame() { return m_last_animation_frame; }
	void set_animation_framerate(float framerate) { m_seconds_per_frame = 1.f / framerate; }


	void render_main_sprite(sf::RenderTarget* render_target);


	bool write_sprites_to_spritesheet(const char* spritesheet_filename) const;

	void set_default_zoom();

private:
	std::unique_ptr<Spritesheet> m_spritesheet{};
	std::vector<sf::Sprite> m_sprites{};
	float m_main_sprite_zoom{ 8.0f };
	int m_current_main_sprite_index{ 0 };
	bool m_valid_sprites{ false };

	sf::Sprite m_drawn_main_sprite_cached{};


	//// Animation
	bool m_playing_animation{ false };
	int m_first_animation_frame{ 0 };
	int m_last_animation_frame{ 0 };
	float m_seconds_per_frame{ 0.033f };
	int m_current_animation_sprite{ 0 };
	float m_seconds_in_current_frame{ 0.f };

};