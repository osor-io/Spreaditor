#pragma once
#include "Colliders.h"
#include "json.hpp"

using nlohmann::json;

void to_json(json& j, const Vec4f& v);
void from_json(const json& j, Vec4f& p);

void to_json(json& j, const ColliderType& v);
void from_json(const json& j, ColliderType& p);

void to_json(json& j, const AttributeType& v);
void from_json(const json& j, AttributeType& p);

void to_json(json& j, const ColliderInstance& v);
void from_json(const json& j, ColliderInstance& p);

void to_json(json& j, const AttributeInstance& v);
void from_json(const json& j, AttributeInstance& p);

void to_json(json& j, const ColliderRect& v);
void from_json(const json& j, ColliderRect& p);

