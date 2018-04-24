#include "ProjectUtils.h"
#include "../sprites/SpriteManager.h"
#include "../colliders/ColliderManager.h"
#include "File.h"

using nlohmann::json;

nlohmann::json project_to_json(const char* path, const char* data_filename, const char* spritesheet_filename) {
	auto json_data = json{};
	json_data["colliders"] = ColliderManager::get().colliders_to_json();

	auto meta = json{};

	auto executable_path = std::string();
	if (path == nullptr || strcmp(path, "") == 0) {
		executable_path = OSManager::get().executable_path();
	}
	else {
		auto executable_path = std::string(path);
	}
	
	meta["path"] = path;
	meta["data_filename"] = std::string(data_filename);
	if (SpriteManager::get().get_spritesheet()) {
		const auto& filename = SpriteManager::get().get_spritesheet()->get_texture().filename;
		meta["spritesheet_filename"] = std::string(spritesheet_filename);
	}
	else {
		CLOG("Generating project JSON without an spritesheet loaded");
		meta["spritesheet_filename"] = nullptr;
	}

	json_data["meta"] = meta;

	// @@DOING @@TODO: Also write the spritesheet data to a json object (do a to_json on the spritesheet)


	return json_data;
}

void project_from_json(nlohmann::json json_data) {

	// @@TODO: Load stuff based on what we write on the function above.

}
