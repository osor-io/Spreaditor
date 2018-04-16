#include "ColliderFactory.h"

ColliderType create_collider_type(const std::string & name) {
    auto type = ColliderType();
    type.name = name;
    return std::move(type);
}

ColliderInstance create_collider_instance(const ColliderType & collider_type, const std::string & name) {
    auto instance = ColliderInstance();
    instance.name = collider_type.name;
    return std::move(instance);
}

AttributeInstance create_attribute_instance(const AttributeType & attribute_type) {
    auto instance = AttributeInstance(attribute_type.type);
    instance.name = attribute_type.name;
    instance.value = attribute_type.default_value;
    return std::move(instance);
}
