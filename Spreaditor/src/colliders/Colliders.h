#pragma once

#include <string>
#include <vector>
#include <set>
#include <variant>
#include <typeindex>
#include <memory>
#include <unordered_map>

#define ATTRIBUTE_VALUE_TYPES std::string, int, float, bool

using AttributeValueType = int;

enum ATTRIBUTE_VALUE_TYPE_ {
	ATTRIBUTE_TYPE_INT = 0,
    ATTRIBUTE_TYPE_FLOAT,
    ATTRIBUTE_TYPE_BOOL,
    ATTRIBUTE_TYPE_STRING,
    ATTRIBUTE_TYPES_COUNT,
};

struct ColliderType;
struct AttributeType;
struct ColliderInstance;
struct AttributeInstance;
struct ColliderRect;
struct Vec4f;
using SpriteIDType = int;
using ColliderCoordinateType = float;


struct Vec4f {
	float x, y, z, w;
};

struct ColliderType {
    std::string name;
    std::set<AttributeType> attributes;
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
    std::variant<ATTRIBUTE_VALUE_TYPES> default_value;
    AttributeValueType type;

    bool operator==(const AttributeType& other) const {
        return (name.compare(other.name) == 0);
    }

    const bool operator<(const AttributeType &other) const {
        return (name < other.name);
    }
};

struct ColliderInstance {
    std::string name;
    std::set<AttributeInstance> attributes;
    std::unordered_map<SpriteIDType, std::vector<ColliderRect>> rects;
    Vec4f color;

    bool operator==(const ColliderInstance& other) const {
        return (name.compare(other.name) == 0);
    }

    const bool operator<(const ColliderInstance &other) const {
        return (name < other.name);
    }
};

struct AttributeInstance {
    std::string name;
    std::variant<ATTRIBUTE_VALUE_TYPES> value;
    AttributeValueType type;

    bool operator==(const AttributeInstance& other) const {
        return (name.compare(other.name) == 0);
    }

    const bool operator<(const AttributeInstance &other) const {
        return (name < other.name);
    }
};

struct ColliderRect {
    ColliderCoordinateType x;
    ColliderCoordinateType y;
    ColliderCoordinateType width;
    ColliderCoordinateType height;
};


/**
    Functions used to hash our collider structures
*/
namespace std {

    template <>
    struct hash<ColliderType>
    {
        size_t operator()(const ColliderType& k) const
        {
            return ((hash<string>()(k.name)));
        }
    };

    template <>
    struct hash<ColliderInstance>
    {
        size_t operator()(const ColliderInstance& k) const
        {
            return ((hash<string>()(k.name)));
        }
    };

    template <>
    struct hash<AttributeType>
    {
        size_t operator()(const AttributeType& k) const
        {
            return ((hash<string>()(k.name)));
        }
    };

    template <>
    struct hash<AttributeInstance>
    {
        size_t operator()(const AttributeInstance& k) const
        {
            return ((hash<string>()(k.name)));
        }
    };

};
