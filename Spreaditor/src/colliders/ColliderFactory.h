#pragma once

#include "Colliders.h"

ColliderType create_collider_type(const std::string& name, const Vec4f& color);

AttributeType create_attribute_type(
    const std::string& name,
    const AttributeValueType& type,
    const std::variant<ATTRIBUTE_VALUE_TYPES>& default_value
);

ColliderInstance create_collider_instance(const ColliderType& collider_type, const std::string& name);

AttributeInstance create_attribute_instance(const AttributeType& attribute_type);

