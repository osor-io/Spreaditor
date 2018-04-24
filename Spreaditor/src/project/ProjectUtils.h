
#include "json.hpp"

nlohmann::json project_to_json(const char* path, const char* data_filename, const char* spritesheet_filename);

bool project_from_json(nlohmann::json json_data, const char* spritesheet_filename = nullptr);