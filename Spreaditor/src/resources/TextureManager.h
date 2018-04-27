#pragma once

#include "ResourceManager.h"

#include <SFML/Graphics.hpp>
#include "File.h"

class TextureManager : public ResourceManager<TextureManager, sf::Texture> {

	friend CRSP<TextureManager>;
	using Base = ResourceManager<TextureManager, sf::Texture>;

public:
	void start_up() override {

	}

	void shut_down() override {
		Base::shut_down();
	}
private:
	sf::Texture* load_resource(const std::string& filename) override {

		auto texture = new sf::Texture();

		/*
		@@NOTE:
		This is fine for now in debug builds since we will end up reading
		reading textures from memory in the final build, likely from some
		packed file (.pak) so this will be read from memory.
		*/
		LOG("Loading Texture Resource: " << filename);
		assert(file_exists(filename.c_str()));

		texture->loadFromFile(filename);
		return texture;
	}

};
