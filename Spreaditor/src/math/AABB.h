#include <SFML\Window.hpp>

struct AABB {

    using Vector = sf::Vector2f;

    enum class Axis {
        x,
        y
    };


#define NORMAL_LEFT     sf::Vector2f{1.0f,0.0f}
#define NORMAL_RIGHT    sf::Vector2f{-1.0f,0.0f}
#define NORMAL_UP       sf::Vector2f{0.0f,-1.0f}
#define NORMAL_DOWN     sf::Vector2f{0.0f,1.0f}


    Vector position;
    Vector extents;

    AABB(Vector position, Vector extents) : position(position), extents(extents) {};

    const bool overlaps(const AABB& other) const;

    const float min(Axis axis) const;

    const float max(Axis axis) const;

    const Vector min() const;

    const Vector max() const;

    const Vector size() const;

    const AABB minkowski_difference(const AABB& other) const;

    const bool has_origin() const;

    const std::tuple<Vector, Vector> closest_point_on_bounds_to_point(const Vector& point) const;

    const std::tuple<float, Vector> ray_intersection_fraction(const Vector& origin, const Vector& direction) const;

    const float ray_intersection_fraction_of_first_ray(
        const Vector& origin_a,
        const Vector& end_a,
        const Vector& origin_b,
        const Vector& end_b) const;

};

AABB operator-(AABB a, const AABB& other);


const bool AABBSweep(
    const AABB::Vector& extents_a,
    const AABB::Vector& a0,
    const AABB::Vector& a1,
    const AABB::Vector& extents_b,
    const AABB::Vector& b0,
    const AABB::Vector& b1,
    float& u0,
    float& u1
);
