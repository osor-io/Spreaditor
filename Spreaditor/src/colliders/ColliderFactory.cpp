#include "ColliderFactory.h"

ColliderType create_collider_type(const std::string & name, const Vec4f& default_color) {
    auto type = ColliderType();
    type.name = name;
    type.default_color = default_color;
    return std::move(type);
}

AttributeType create_attribute_type(
    const std::string& name,
    const AttributeValueType& type,
    const std::variant<ATTRIBUTE_VALUE_TYPES>& default_value
) {
    auto attribute_type = AttributeType{};
    attribute_type.name = name;
    attribute_type.default_value = default_value;
    attribute_type.type = type;
    return std::move(attribute_type);
}

ColliderInstance create_collider_instance(const ColliderType & collider_type, const std::string & name, Vec4f * color) {
    auto instance = ColliderInstance();
    instance.name = name;
    if (color) {
        instance.color = *color;
    }
    else {
        instance.color = collider_type.default_color;
    }
    for (const auto& attr : collider_type.attributes) {
        instance.attributes.insert(create_attribute_instance(attr));
    }
    return std::move(instance);
}

AttributeInstance create_attribute_instance(const AttributeType & attribute_type) {
    auto instance = AttributeInstance{};
    instance.name = attribute_type.name;
    instance.value = attribute_type.default_value;
    instance.type = attribute_type.type;
    return std::move(instance);
}
