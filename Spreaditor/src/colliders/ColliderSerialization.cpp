#include "ColliderSerialization.h"

void to_json(json & j, const ColliderType & v) {
	j = json{
		{ "name", v.name }
	};

	j["attributes"] = v.attributes;
	j["default_color"] = v.default_color;
}

void from_json(const json & j, ColliderType & p) {
	p.name = j["name"].get<std::string>();
	p.attributes = j["attributes"].get<std::set<AttributeType>>();
	p.default_color = j["default_color"].get<Vec4f>();
}

void to_json(json & j, const AttributeType & v) {

	j = json{
		{ "name", v.name }
	};

	switch (v.type) {
	case ATTRIBUTE_TYPE_INT:
		j["default_value"] = std::get<int>(v.default_value);
		j["type"] = "integer";
		break;
	case ATTRIBUTE_TYPE_FLOAT:
		j["default_value"] = std::get<float>(v.default_value);
		j["type"] = "float";
		break;
	case ATTRIBUTE_TYPE_BOOL:
		j["default_value"] = std::get<bool>(v.default_value);
		j["type"] = "bool";
		break;
	case ATTRIBUTE_TYPE_STRING:
		j["default_value"] = std::get<std::string>(v.default_value);
		j["type"] = "string";
		break;
	default:
		assert(false); // Wrong type of attribute
	}

}

void from_json(const json & j, AttributeType & v) {

	v.name = j["name"].get<std::string>();

	std::string type_string = j["type"];

	if (type_string.compare("integer")) {
		v.type = ATTRIBUTE_TYPE_INT;
		v.default_value = j["default_value"].get<int>();
	}
	else if (type_string.compare("float")) {
		v.type = ATTRIBUTE_TYPE_FLOAT;
		v.default_value = j["default_value"].get<float>();
	}
	else if (type_string.compare("bool")) {
		v.type = ATTRIBUTE_TYPE_BOOL;
		v.default_value = j["default_value"].get<bool>();
	}
	else if (type_string.compare("string")) {
		v.type = ATTRIBUTE_TYPE_STRING;
		v.default_value = j["default_value"].get<std::string>();
	}
	else {
		assert(false); // Wrong type of attribute
	}

}

void to_json(json & j, const ColliderInstance & v) {
	j = json{
		{ "name", v.name }
	};

	j["attributes"] = v.attributes;

	auto& j_rects = j["rects"];
	j_rects = json(json::value_t::object);

	for (const auto& elem : v.rects) {
		auto const string_key = std::to_string(elem.first);
		j_rects[string_key] = elem.second;
	}

	j["color"] = v.color;
}

void from_json(const json & j, ColliderInstance & p) {
	p.name = j["name"].get<std::string>();
	p.attributes = j["attributes"].get<std::set<AttributeInstance>>();

	for (const auto& elem : j["rects"].items()) {
		auto const int_key = stoi(elem.key());
		p.rects[int_key] = elem.value().get<std::vector<ColliderRect>>();
	}

	p.color = j["color"].get<Vec4f>();
}

void to_json(json & j, const AttributeInstance & v) {
	j = json{
		{ "name", v.name }
	};

	switch (v.type) {
	case ATTRIBUTE_TYPE_INT:
		j["value"] = std::get<int>(v.value);
		j["type"] = "integer";
		break;
	case ATTRIBUTE_TYPE_FLOAT:
		j["value"] = std::get<float>(v.value);
		j["type"] = "float";
		break;
	case ATTRIBUTE_TYPE_BOOL:
		j["value"] = std::get<bool>(v.value);
		j["type"] = "bool";
		break;
	case ATTRIBUTE_TYPE_STRING:
		j["value"] = std::get<std::string>(v.value);
		j["type"] = "string";
		break;
	default:
		assert(false); // Wrong type of attribute
	}
}

void from_json(const json & j, AttributeInstance & v) {

	v.name = j["name"].get<std::string>();

	std::string type_string = j["type"];

	if (type_string.compare("integer")) {
		v.type = ATTRIBUTE_TYPE_INT;
		v.value = j["value"].get<int>();
	}
	else if (type_string.compare("float")) {
		v.type = ATTRIBUTE_TYPE_FLOAT;
		v.value = j["value"].get<float>();
	}
	else if (type_string.compare("bool")) {
		v.type = ATTRIBUTE_TYPE_BOOL;
		v.value = j["value"].get<bool>();
	}
	else if (type_string.compare("string")) {
		v.type = ATTRIBUTE_TYPE_STRING;
		v.value = j["value"].get<std::string>();
	}
	else {
		assert(false); // Wrong type of attribute
	}

}

void to_json(json & j, const ColliderRect & v) {
	j = json{ { "x", v.x },{ "y", v.y },{ "width", v.width },{ "height", v.height } };
}

void from_json(const json & j, ColliderRect & p) {
	p.x = j["x"];
	p.y = j["y"];
	p.width = j["width"];
	p.height = j["height"];
}

void to_json(json & j, const Vec4f & v) {
	j = json{ { "x", v.x },{ "y", v.y },{ "z", v.z },{ "w", v.w } };
}

void from_json(const json & j, Vec4f & p) {
	p.x = j["x"];
	p.y = j["y"];
	p.z = j["z"];
	p.w = j["w"];
}
