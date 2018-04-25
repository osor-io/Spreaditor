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

	json_data["spritesheet"] = SpriteManager::get().get_spritesheet()->to_json();

	return json_data;
}

bool project_from_json(nlohmann::json json_data, const char* external_spritesheet_filename) {

	if (json_data.find("meta") == json_data.end()
		|| json_data["meta"].is_null()) {
		CLOG_ERROR("We couldn't read the meta attributes from the provided JSON data");
		return false;
	}
	if (json_data.find("colliders") == json_data.end()
		|| json_data["colliders"].is_null()) {
		CLOG_ERROR("We couldn't read the colliders from the provided JSON data");
		return false;
	}
	if (json_data.find("spritesheet") == json_data.end()
		|| json_data["spritesheet"].is_null()) {
		CLOG_ERROR("We couldn't read the spritesheet from the provided JSON data");
		return false;
	}

	json jmeta = json_data["meta"];
	json jcolliders = json_data["colliders"];
	json jspritesheet = json_data["spritesheet"];

	auto spritesheet_name = std::string{};
	{
		if (external_spritesheet_filename) {
			spritesheet_name = std::string(external_spritesheet_filename);
		}
		else {
			auto jspritesheet_name = jmeta["spritesheet_filename"];
			if (!jspritesheet_name.is_null()) {
				spritesheet_name = jspritesheet_name.get<std::string>();
			}
			else {
				CLOG_ERROR("There is no spritesheet to load in the provided JSON data");
				return false;
			}
		}
	}

	auto path = std::string{};
	{
		auto jpath = jmeta["path"];
		if (!jpath.is_null()) {
			path = jpath.get<std::string>();
		}
		else {
			CLOG("There is no path attribute to read in the provided JSON data, we will try to use the current executable path");
			path = OSManager::get().executable_path();
		}
	}

	auto loaded_colliders = ColliderManager::get().colliders_from_json(jcolliders);

	if (!loaded_colliders) {
		CLOG_ERROR("We couldn't load the provided collider data");
		return false;
	}

	auto loaded_spritesheet = SpriteManager::get().load_spritesheet_from_json(path + spritesheet_name, jspritesheet);

	if (!loaded_spritesheet) {
		CLOG_ERROR("We couldn't load the provided spritesheet");
		return false;
	}

	return true;
}
