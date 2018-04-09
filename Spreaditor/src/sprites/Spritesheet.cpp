#include "Spritesheet.h"
#include "../resources/TextureManager.h"
#include "Debug.h"
#include "File.h"
#include <tuple>

Spritesheet::Spritesheet(const std::string & texture_filename, SpritesheetMorphology type) {

    if (m_image_cache == nullptr) {
        m_image_cache = new Image();
        m_image_cache->loadFromFile(texture_filename);
    }

    int rows, cols;
    std::tie(rows, cols) = infer_rows_and_columns(*m_image_cache);

    /*
    @@NOTE

    I agree, this is kind of hacky and maybe not the best option.
    A best way to do it would be to refactor the code inside these
    specific constructors into other functions and then have each
    constructor only call the necessary functions, keeping the constuctors
    self-contained and independent from each other.

    I did it this way because I am experimenting with these kind of things
    to find out wether this is a good/decent solution for some specific
    cases like this one where you sort of have what I'll call "cascading"
    constructors, where one with less parameters calculates some stuff and
    then gives information to the next one until you reach the bottom of 
    the "cascade" where you have all of the required parameters to finally
    create the object correctly.
    */
    *this = Spritesheet(texture_filename, rows, cols, type);
}

Spritesheet::Spritesheet(const std::string & texture_filename, int rows, int cols, SpritesheetMorphology type) {

    if (m_image_cache == nullptr) {
        m_image_cache = new Image();
        m_image_cache->loadFromFile(texture_filename);
    }

    int sprite_width;
    int sprite_height;

    if (type == SpritesheetMorphology::SQUARE) {
        sprite_width = sprite_height = m_image_cache->getSize().x / cols;
    }
    else  if (type == SpritesheetMorphology::UNIFORMLY_PACKED) {
        sprite_width = m_image_cache->getSize().x / cols;
        sprite_height = m_image_cache->getSize().y / rows;
    }

    *this = Spritesheet(texture_filename, rows, cols, sprite_width, sprite_height, type);
}

Spritesheet::Spritesheet(const std::string & texture_filename, int rows, int cols, int sprite_width, int sprite_height, SpritesheetMorphology type) :
    m_sprite_type(type),
    m_rows(rows),
    m_cols(cols),
    m_sprite_width(sprite_width),
    m_sprite_height(sprite_height),
    m_texture_filename(texture_filename),
    m_texture(TextureManager::get().get_required_resource(texture_filename))
{
    if (m_image_cache != nullptr) delete m_image_cache;

    if (!m_texture) {
        CLOG_ERROR("We couldn't load the texture from the file " << m_texture_filename);
        return;
    }

    fill_sprite_container();
}

Spritesheet::~Spritesheet() {
    if (m_texture) {
        TextureManager::get().release_required_resource(m_texture_filename);
        m_texture = nullptr;
    }
}


std::tuple<int, int> Spritesheet::infer_rows_and_columns(const Image& spritesheet_image) {

    auto res = std::tuple<int, int>();

    auto array_clear_rows = std::vector<bool>(spritesheet_image.getSize().y);
    auto array_clear_cols = std::vector<bool>(spritesheet_image.getSize().x);

    for (auto i = 0; i < array_clear_rows.size(); ++i) {
        array_clear_rows[i] = line_empty(spritesheet_image, i, Line::ROW);
    }

    for (auto i = 0; i < array_clear_cols.size(); ++i) {
        array_clear_cols[i] = line_empty(spritesheet_image, i, Line::COLUMN);
    }

    return std::make_tuple(
        count_continuous_regions(array_clear_rows),
        count_continuous_regions(array_clear_cols)
    );
}

bool Spritesheet::line_empty(const Image& image, int line, Line type_to_check) {
    /*
    @@NOTE

    We will take the RGBA value of the (0,0) position in the texture
    as the background colour to detect sprites.
    */

    auto background_color = image.getPixel(0, 0);

    if (type_to_check == Line::ROW) {
        for (auto i = 0u; i < image.getSize().x; ++i) {
            if (image.getPixel(i, line) != background_color) {
                return false;
            }
        }
    }
    else if (type_to_check == Line::COLUMN) {
        for (auto i = 0u; i < image.getSize().y; ++i) {
            if (image.getPixel(line, i) != background_color) {
                return false;
            }
        }
    }

    return true;
}

int Spritesheet::count_continuous_regions(const std::vector<bool>& vector, int threshold) {
    auto count = 0;

    for (auto i = 0; i < vector.size(); ++i) {

        if (vector[i]) {
            int empty_count = 1;
            while (++i < vector.size() && vector[i]) {
                ++empty_count;
            }
            if (empty_count >= threshold)
                ++count;
        }
        else {

        }

    }

    return count - 1;
}

void Spritesheet::fill_sprite_container() {
    for (auto row = 0; row < m_rows; ++row) {
        for (auto col = 0; col < m_cols; ++col) {
            /*
            @@NOTE

            This might insert some "empty" sprites that are at the
            end of the last row, for example, if there are 3 rows and
            columns but only 7 sprites, two sprites that reference the
            last two positions (3,2) & (3,3) will me inserted but they
            will only contain the background color and not really any sprite.
            */
            m_sprite_container.push_back(
                Sprite(
                    *m_texture,
                    IntRect(
                        col*m_sprite_width,
                        row*m_sprite_height,
                        m_sprite_width,
                        m_sprite_height
                    ))
            );
        }
    }
}
