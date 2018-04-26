#include "SpriteManager.h"
#include "../window/WindowManager.h"
#include "../gui/GUIManager.h"
#include "../tools/ToolsManager.h"
#include "../render/RenderManager.h"
#include "../math/OtherMath.h"
#include "../time/TimeManager.h"

SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

void SpriteManager::start_up() {}

void SpriteManager::shut_down() {}

bool SpriteManager::load_spritesheet(const char * filename) {
	m_spritesheet = std::make_unique<Spritesheet>(filename, Spritesheet::SpritesheetMorphology::UNIFORMLY_PACKED);
	m_sprites = m_spritesheet->get_sprites();
	m_valid_sprites = m_spritesheet->is_valid();
	set_default_zoom();
	return m_valid_sprites;
}

bool SpriteManager::load_spritesheet(const char * filename, int rows, int cols){
	m_spritesheet = std::make_unique<Spritesheet>(filename, rows, cols, Spritesheet::SpritesheetMorphology::UNIFORMLY_PACKED);
	m_sprites = m_spritesheet->get_sprites();
	m_valid_sprites = m_spritesheet->is_valid();
	set_default_zoom();
	return m_valid_sprites;
}

bool SpriteManager::load_spritesheet(const char * filename, int rows, int cols, int width, int height){
	m_spritesheet = std::make_unique<Spritesheet>(filename, rows, cols, width, height, Spritesheet::SpritesheetMorphology::UNIFORMLY_PACKED);
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


bool SpriteManager::load_spritesheet_from_json(const std::string & filename, const json & data) {

	if (m_spritesheet) {
		m_spritesheet->from_json(filename, data);
	}
	else {
		m_spritesheet = std::make_unique<Spritesheet>(filename, data);
	}

	m_sprites = m_spritesheet->get_sprites();
	m_valid_sprites = m_spritesheet->is_valid();
	set_default_zoom();
	return m_valid_sprites;
}

void SpriteManager::render_main_sprite(sf::RenderTarget* render_target) {

	if (!m_valid_sprites) return;

	auto sprite_index_to_draw{ -1 };

	//// ANIMATION
	{
		if (m_last_animation_frame < m_first_animation_frame) {
			m_last_animation_frame = m_first_animation_frame;
		}

		// We select the sprite based on animation if we have to
		if (m_playing_animation) {

			static auto previous_ms = TimeManager::get().cycles_to_ms(TimeManager::get().query_cycle_counter());
			auto current_ms = TimeManager::get().cycles_to_ms(TimeManager::get().query_cycle_counter());
			auto advanced_ms = current_ms - previous_ms;
			previous_ms = current_ms;
			m_seconds_in_current_frame += advanced_ms / 1000.f;
			auto difference = m_seconds_in_current_frame - m_seconds_per_frame;

			/*
			@@TODO
			
			Check here possible inconsistency with how much time we 
			spend in each frame. Seen sometimes with high framerates (60+).
			*/


			if (difference > 0.f) {
				m_seconds_in_current_frame = fmod(difference, m_seconds_per_frame);
				++m_current_animation_sprite;
				if (m_current_animation_sprite > m_last_animation_frame) {
					m_current_animation_sprite = m_first_animation_frame;
				}
			}

			sprite_index_to_draw = m_current_animation_sprite;
		}
		else {
			sprite_index_to_draw = m_current_main_sprite_index;
		}
	}


	if (sprite_index_to_draw >= m_sprites.size() || sprite_index_to_draw < 0) return;

	//// DRAWING
	{

		m_drawn_main_sprite_cached = m_sprites[sprite_index_to_draw];

		m_drawn_main_sprite_cached.setOrigin(m_drawn_main_sprite_cached.getTextureRect().width / 2.0f, m_drawn_main_sprite_cached.getTextureRect().height / 2.0f);
		m_drawn_main_sprite_cached.setScale(m_main_sprite_zoom, m_main_sprite_zoom);
		auto bounds = m_drawn_main_sprite_cached.getGlobalBounds();

		/*
		@@TODO @@MAYBE

		Maybe also offset the image horizontally based on the current width of the
		collider explorer and the tools bar so we can center it a bit nicely.

		*/

		auto timeline_offset = GUIManager::get().get_timeline_height();
		auto screen_height = render_target->getView().getSize().y;
		auto vertical_offset = GUIManager::get().get_main_menu_height() + ToolsManager::get().tools_options_bar_height();

		auto vertical_position =
			vertical_offset
			+
			(((screen_height - vertical_offset) - timeline_offset - bounds.height) / 2.f)
			+
			bounds.height / 2.0f;

		m_drawn_main_sprite_cached.setPosition(render_target->getView().getSize().x / 2.f, vertical_position);

		render_target->draw(m_drawn_main_sprite_cached);

		bounds = m_drawn_main_sprite_cached.getGlobalBounds();
		auto border = sf::RectangleShape(sf::Vector2f(bounds.width, bounds.height));
		border.setFillColor(sf::Color::Transparent);
		border.setPosition(sf::Vector2f(bounds.left, bounds.top));
		border.setOutlineThickness(2);
		border.setOutlineColor(sf::Color::White);

		render_target->draw(border);
	}

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

	const auto extra_padding = ImGui::GetTextLineHeightWithSpacing();

	auto vertical_space =
		ImGui::GetIO().DisplaySize.y
		- GUIManager::get().get_timeline_height()
		- GUIManager::get().get_main_menu_height()
		- ToolsManager::get().tools_options_bar_height()
		- extra_padding * 2;

	auto horizontal_space = ImGui::GetIO().DisplaySize.y * 0.7f;

	auto desired_zoom_x = horizontal_space / real_sprite_width;
	auto desired_zoom_y = vertical_space / real_sprite_height;

	m_main_sprite_zoom = desired_zoom_x < desired_zoom_y ? desired_zoom_x : desired_zoom_y;
}
