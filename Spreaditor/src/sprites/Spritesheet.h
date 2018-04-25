#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <json.hpp>
#include "../resources/TextureManager.h"

class Spritesheet {
	using Sprite = sf::Sprite;
	using IntRect = sf::IntRect;
	using Texture = sf::Texture;
	using Image = sf::Image;
	using json = nlohmann::json;

	struct TexturePacked {
		Texture* texture;
		std::string filename;
		bool external{ true };


		TexturePacked() : texture(nullptr), filename() {}

		TexturePacked(const TexturePacked& other) {
			external = other.external;
			filename = other.filename;
			if (external) {
				texture = TextureManager::get().get_required_resource(filename);
			}
			else {
				texture = new Texture(*other.texture);
			}
		}

		TexturePacked(const Image& image) {
			texture = new Texture();
			texture->loadFromImage(image);
			filename = "INTERNAL";
			external = false;
		}

		TexturePacked(Texture* o_texture, const std::string& o_filename) {
			texture = o_texture;
			filename = o_filename;
		}

		~TexturePacked() {
			clear();
		}


		TexturePacked & operator=(TexturePacked && other) {
			texture = other.texture;
			filename = other.filename;
			external = other.external;
			other.texture = nullptr;
			other.filename = "";
			return *this;
		}

		void clear() {
			if (external) {
				if (texture) {
					TextureManager::get().release_required_resource(filename);
					texture = nullptr;
				}
			}
			else {
				if (texture) {
					delete texture;
					texture = nullptr;
				}
			}
			filename = "";
		}

	};

public:

	enum class SpritesheetMorphology {
		SQUARE,
		UNIFORMLY_PACKED,
	};

	Spritesheet(const std::string& filename, const json& data);
	Spritesheet(const std::vector<std::string>& texture_filenames);
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
	bool is_saved_to_file() const { return m_saved_to_file; }
	const std::string& get_saved_filename() const { return m_saved_filename; }
	const TexturePacked& get_texture() const { return m_texture; }

	bool write_to_file(const std::string& file);

	static bool write_to_file(const std::string& file, const std::vector<Sprite>& sprites);

	json to_json();
	bool from_json(const std::string& filename, const json& data);

private: // Member Functions

	void construct(const std::string& texture_filename, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
	void construct(const std::string& texture_filename, int rows, int cols, SpritesheetMorphology type = SpritesheetMorphology::SQUARE);
	void construct(const std::string& texture_filename, int rows, int cols, int sprite_width, int sprite_height, SpritesheetMorphology type = SpritesheetMorphology::SQUARE, bool regenerate_image = true);

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

	void refill_sprite_container();

	bool fill_image_cache(const std::string& filename);

	static Image write_to_image(const std::vector<Spritesheet::Sprite>& sprites,
		unsigned int& cell_side_pixel_length,
		unsigned int& total_rows_and_cols);

	void generate_image();

private: // Member Attributes

	std::vector<sf::Sprite> m_sprite_container{};
	TexturePacked m_texture;
	Image m_image;

	int m_rows{};
	int m_cols{};
	int m_sprite_width{};
	int m_sprite_height{};
	bool m_valid{ true };
	SpritesheetMorphology m_sprite_type{};

	bool m_saved_to_file{ false };
	std::string m_saved_filename{ "NOT SAVED IN FILE" };

	Image* m_image_cache{ nullptr };
};

