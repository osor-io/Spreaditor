#pragma once

#include "Colliders.h"

ColliderType create_collider_type(const std::string& name, const Vec4f& color);

template<typename T>
AttributeType create_attribute_type(
    const std::string& name,
    const T& default_value
) {
    return Attribute(name, default_value, std::type_index(typeid(T)));
}

ColliderInstance create_collider_instance(const ColliderType& collider_type, const std::string& name);

AttributeInstance create_attribute_instance(const AttributeType& attribute_type);

