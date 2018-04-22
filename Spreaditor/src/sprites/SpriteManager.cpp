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

    CLOG("Creating spritesheet with current sprites");

    auto max_sprite_width = 0;
    auto max_sprite_height = 0;
    auto sprite_count = m_sprites.size();

    for (const auto& sprite : m_sprites) {
        if (sprite.getTextureRect().width > max_sprite_width)
            max_sprite_width = sprite.getTextureRect().width;

        if (sprite.getTextureRect().height > max_sprite_height)
            max_sprite_height = sprite.getTextureRect().height;
    }

    auto spritesheet_side_pixel_length = next_power_of_2(sqrt(max_sprite_width*max_sprite_height*sprite_count));

    auto cell_side_pixel_length = next_power_of_2(max_sprite_width > max_sprite_height ? max_sprite_width : max_sprite_height);

    auto total_rows_and_cols = spritesheet_side_pixel_length / cell_side_pixel_length;

    CLOG("\tSpritesheet image size: " << spritesheet_side_pixel_length << "x" << spritesheet_side_pixel_length);
    CLOG("\tSpritesheet cell size: " << cell_side_pixel_length);


    auto spritesheet_image = sf::Image();
    spritesheet_image.create(spritesheet_side_pixel_length, spritesheet_side_pixel_length, sf::Color(.0f, .0f, .0f, .0f));

    {
        auto row = 0;
        auto col = 0;
        for (const auto& sprite : m_sprites) {

            auto texture = sprite.getTexture();
            assert(texture != nullptr);
            
            auto texture_image = texture->copyToImage();

            auto dest_x = row * cell_side_pixel_length + ((cell_side_pixel_length - sprite.getTextureRect().width)/2.f);
            auto dest_y = col * cell_side_pixel_length + ((cell_side_pixel_length - sprite.getTextureRect().height)/2.f);

            spritesheet_image.copy(texture_image, dest_x, dest_y, sprite.getTextureRect(), true);

            if (++row >= total_rows_and_cols) {
                row = 0;
                ++col;
                col = col < total_rows_and_cols ? col : 0;
            }
        }
    }

    if (!spritesheet_image.saveToFile(spritesheet_filename)) {
        CLOG_ERROR("Error saving the pixels to the file: " << spritesheet_filename);
        return false;
    }
    CLOG("Wrote spritesheet to file: " << spritesheet_filename);
    return true;
}
