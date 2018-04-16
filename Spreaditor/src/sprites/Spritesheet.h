#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>


/*
@@TODO

- Add constructor to create a spritesheet based on a vector of sprites
*/

class Spritesheet {
    using Sprite = sf::Sprite;
    using IntRect = sf::IntRect;
    using Texture = sf::Texture;
    using Image = sf::Image;

public:

    enum class SpritesheetMorphology {
        SQUARE,
        UNIFORMLY_PACKED,
    };

    Spritesheet(const std::string& texture_filename, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
    Spritesheet(const std::string& texture_filename, int rows, int cols, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
    Spritesheet(const std::string& texture_filename, int rows, int cols, int sprite_width, int sprite_height, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
    ~Spritesheet();


    const std::vector<sf::Sprite>& get_sprites() const { return m_sprite_container; }
    int get_rows() const { return m_rows; }
    int get_cols() const { return m_cols; }
    int get_sprite_width() const { return m_sprite_width; }
    int get_sprite_height() const { return m_sprite_height; }
    bool is_valid() const { return m_valid; }

private: // Member Functions

    void construct(const std::string& texture_filename, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
    void construct(const std::string& texture_filename, int rows, int cols, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
    void construct(const std::string& texture_filename, int rows, int cols, int sprite_width, int sprite_height, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);

    std::tuple<int, int> infer_rows_and_columns(const Image&);

    enum class Line {
        ROW,
        COLUMN
    };

    bool line_empty(const Image& image, int line, Line type_to_check);

    /**

    We allow for regions that have empty lines up to 4 pixels wide,
    this is because a sprite drawn with pieces that are not all
    connected to each other might be detected as new sprites incorrectly.
    */
    int count_continuous_regions(const std::vector<bool>& vector, int threshold = 4);

    void fill_sprite_container();

    bool fill_image_cache(const std::string& filename);

private: // Member Attributes

    std::vector<sf::Sprite> m_sprite_container{};
    Texture* m_texture{ nullptr };
    std::string m_texture_filename{};

    int m_rows{};
    int m_cols{};
    int m_sprite_width{};
    int m_sprite_height{};
    bool m_valid{ true };
    SpritesheetMorphology m_sprite_type{};


    Image* m_image_cache{ nullptr };
};

