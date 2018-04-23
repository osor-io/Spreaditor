#include "ProjectUtils.h"
#include "../sprites/SpriteManager.h"
#include "../colliders/ColliderManager.h"
#include "File.h"

using nlohmann::json;

nlohmann::json project_to_json() {
	auto json_data = json{};
	json_data["colliders"] = ColliderManager::get().colliders_to_json();
	if (SpriteManager::get().get_spritesheet()) {
		const auto& filename = SpriteManager::get().get_spritesheet()->get_texture().filename;
		json_data["spritesheet_full_path"] = filename;
		json_data["spritesheet"] = extract_filename(filename.c_str());
	}
	else {
		json_data["spritesheet_full_path"] = nullptr;
		json_data["spritesheet"] = nullptr;
	}

	return json_data;
}

void project_from_json(nlohmann::json json_data) {

	// @@TODO: Load stuff based on what we write on the function above.

}
