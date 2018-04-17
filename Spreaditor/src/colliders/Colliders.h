#pragma once

#include <string>
#include <vector>
#include <variant>
#include <typeindex>
#include <memory>
#include <unordered_map>

#define ATTRIBUTE_TYPES std::string, int, float, bool

struct Vec4f;
struct ColliderType;
struct AttributeType;
struct ColliderInstance;
struct AttributeInstance;
struct ColliderRect;
using SpriteIDType = int;
using ColliderCoordinateType = float;


struct Vec4f {
    float x, y, z, w;
};

struct ColliderType {
    std::string name;
    std::vector<AttributeType> attributes;
    Vec4f default_color;

    bool operator==(const ColliderType& other) const {
        return (name.compare(other.name) == 0);
    }

    const bool operator<(const ColliderType &other) const {
        return (name < other.name);
    }
};

struct AttributeType {
    std::string name;
    std::variant<ATTRIBUTE_TYPES> default_value;
    std::type_index type;

    bool operator==(const AttributeType& other) const {
        return (name.compare(other.name) == 0);
    }
};

struct ColliderInstance {
    std::string name;
    std::vector<AttributeInstance> attributes;
    std::unordered_map<SpriteIDType, std::vector<ColliderRect>> rects;
    Vec4f color;

    bool operator==(const ColliderInstance& other) const {
        return (name.compare(other.name) == 0);
    }
};

struct AttributeInstance {
    AttributeInstance(std::type_index type) : type(type) {}
    std::string name;
    std::variant<ATTRIBUTE_TYPES> value;
    std::type_index type;

    bool operator==(const AttributeInstance& other) const {
        return (name.compare(other.name) == 0);
    }
};

struct ColliderRect {
    ColliderCoordinateType x;
    ColliderCoordinateType y;
    ColliderCoordinateType width;
    ColliderCoordinateType height;
};

namespace std {

    template <>
    struct hash<ColliderType>
    {
        size_t operator()(const ColliderType& k) const
        {
            // Compute individual hash values for two data members and combine them using XOR and bit shifting
            return ((hash<string>()(k.name)));
        }
    };

    template <>
    struct hash<ColliderInstance>
    {
        size_t operator()(const ColliderInstance& k) const
        {
            // Compute individual hash values for two data members and combine them using XOR and bit shifting
            return ((hash<string>()(k.name)));
        }
    };

};