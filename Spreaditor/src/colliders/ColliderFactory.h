#pragma once

#include "Colliders.h"

#define MAX_COLLIDER_STRING_SIZE 512

ColliderType create_collider_type(const std::string& name, const Vec4f& color);

AttributeType create_attribute_type(
    const std::string& name,
    const AttributeValueType& type,
    const std::variant<ATTRIBUTE_VALUE_TYPES>& default_value
);

ColliderInstance create_collider_instance(const ColliderType& collider_type, const std::string& name, Vec4f * color = nullptr);

AttributeInstance create_attribute_instance(const AttributeType& attribute_type);

