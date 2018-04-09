#include "AABB.h"
#include <cassert>
#include <array>
#include "Debug.h"
#include "../math/Vector.h"

const bool AABB::overlaps(const AABB& other) const {
    const Vector t = other.position - position;
    return  (
        fabs(t.x) <= (extents.x + other.extents.x) &&
        fabs(t.y) <= (extents.y + other.extents.y)
        );
}

const float AABB::min(AABB::Axis axis) const {
    if (axis == AABB::Axis::x) {
        return position.x - extents.x;
    }
    else if (axis == AABB::Axis::y) {
        return position.y - extents.y;
    }
    assert(false);
}

const float AABB::max(AABB::Axis axis) const {
    if (axis == AABB::Axis::x) {
        return position.x + extents.x;
    }
    else if (axis == AABB::Axis::y) {
        return position.y + extents.y;
    }
    assert(false);
}

const AABB::Vector AABB::min() const {
    return position - extents;
}
const AABB::Vector AABB::max() const {
    return position + extents;
}

const AABB::Vector AABB::size() const {
    return extents * 2.0f;
}

const AABB AABB::minkowski_difference(const AABB & other) const {
    auto top_left = min() - other.max();
    auto fullsize = size() + other.size();
    return AABB(top_left + (fullsize / 2.0f), fullsize / 2.0f);
}

const bool AABB::has_origin() const {
    return (
        min().x <= 0.f &&
        max().x >= 0.f &&
        min().y <= 0.f &&
        max().y >= 0.f
        );
}

const std::tuple<AABB::Vector, AABB::Vector> AABB::closest_point_on_bounds_to_point(const Vector & point) const {

    const auto l_max = max();
    const auto l_min = min();

    auto min_dist = fabs(point.x - l_min.x);
    auto normal = NORMAL_LEFT;
    auto bounds_point = Vector(l_min.x, point.y);

    if (fabs(l_max.x - point.x) < min_dist) {
        min_dist = fabs(l_max.x - point.x);
        normal = NORMAL_RIGHT;
        bounds_point = Vector(l_max.x, point.y);
    }
    if (fabs(l_max.y - point.y) < min_dist) {
        min_dist = fabs(l_max.y - point.y);
        normal = NORMAL_UP;
        bounds_point = Vector(point.x, l_max.y);
    }
    if (fabs(l_min.y - point.y) < min_dist) {
        min_dist = fabs(l_min.y - point.y);
        normal = NORMAL_DOWN;
        bounds_point = Vector(point.x, l_min.y);
    }

    return std::make_tuple(bounds_point, normal);
}

const std::tuple<float, AABB::Vector>  AABB::ray_intersection_fraction(const Vector & origin, const Vector & direction) const {

    const auto end = origin + direction;

    auto normal = Vector{};
    
    auto local_min = min();
    auto local_max = max();

    auto current_min = float{};
    
    auto f1 = ray_intersection_fraction_of_first_ray(
        origin,
        end,
        Vector(local_min.x, local_min.y),
        Vector(local_min.x, local_max.y));

    current_min = f1;
    normal = NORMAL_LEFT;

    auto f2 = ray_intersection_fraction_of_first_ray(
        origin,
        end,
        Vector(local_min.x, local_max.y),
        Vector(local_max.x, local_max.y));

    if (f2 < current_min) {
        current_min = f2;
        normal = NORMAL_DOWN;
    }
    
    auto f3 = ray_intersection_fraction_of_first_ray(
        origin,
        end,
        Vector(local_max.x, local_max.y),
        Vector(local_max.x, local_min.y));

    if (f3 < current_min) {
        current_min = f3;
        normal = NORMAL_RIGHT;
    }

    auto f4 = ray_intersection_fraction_of_first_ray(
        origin,
        end,
        Vector(local_max.x, local_min.y),
        Vector(local_min.x, local_min.y));

    if (f4 < current_min) {
        current_min = f4;
        normal = NORMAL_UP;
    }
    

    return std::make_tuple(current_min,normal);

}

//https://hamaluik.com/posts/swept-aabb-collision-using-minkowski-difference/

const float AABB::ray_intersection_fraction_of_first_ray(const Vector & origin_a, const Vector & end_a, const Vector & origin_b, const Vector & end_b) const {

    auto r = end_a - origin_a;
    auto s = end_b - origin_b;

    auto numerator = cross((origin_b - origin_a), r);
    auto denominator = cross(r, s);

    if (numerator == 0.f && denominator == 0.f) {

        // Lines are colinear
        return std::numeric_limits<float>::infinity();
    }
    else if (denominator == 0.f) {

        //Lines are parallel
        return std::numeric_limits<float>::infinity();
    }
    //(0,0) (-5,0.8) (-4.56085205, 58.9273376) (-4.56085205, -113.072662)
    auto u = numerator / denominator;
    auto t = cross((origin_b - origin_a), s) / denominator;

    if ((t >= 0.f) && (t <= 1.f) && (u >= 0.f) && (u <= 1.f)) return t;

    return std::numeric_limits<float>::infinity();
}

static const std::array<AABB::Axis, 2> all_axis{ AABB::Axis::x, AABB::Axis::y };

AABB operator-(AABB a, const AABB & other) {
    return a.minkowski_difference(other);
}

const bool AABBSweep(
    const AABB::Vector & ea,
    const AABB::Vector & a0,
    const AABB::Vector & a1,
    const AABB::Vector & eb,
    const AABB::Vector & b0,
    const AABB::Vector & b1,
    float & u0,
    float & u1) {

    const auto a = AABB(a0, ea);
    const auto b = AABB(b0, eb);

    const auto va = a1 - a0;
    const auto vb = b1 - b0;

    const auto v = vb - va;

    auto u_0 = AABB::Vector(0.f, 0.f);
    auto u_1 = AABB::Vector(1.f, 1.f);


    if (a.overlaps(b)) {
        u0 = u1 = 0.0f;
        return true;
    }

    auto axis = AABB::Axis::x;

    if (a.max(axis) < b.min(axis) && v.x < 0) {
        u_0.x = (a.max(axis) - b.min(axis)) / v.x;
    }
    else if (b.max(axis) < a.min(axis) && v.x > 0) {
        u_0.x = (a.min(axis) - b.max(axis)) / v.x;
    }

    if (b.max(axis) > a.min(axis) && v.x < 0) {
        u_1.x = (a.min(axis) - b.max(axis)) / v.x;
    }
    else if (a.max(axis) > b.min(axis) && v.x > 0) {
        u_1.x = (a.max(axis) - b.min(axis)) / v.x;
    }

    axis = AABB::Axis::y;

    if (a.max(axis) < b.min(axis) && v.y < 0) {
        u_0.x = (a.max(axis) - b.min(axis)) / v.y;
    }
    else if (b.max(axis) < a.min(axis) && v.y > 0) {
        u_0.x = (a.min(axis) - b.max(axis)) / v.y;
    }

    if (b.max(axis) > a.min(axis) && v.y < 0) {
        u_1.x = (a.min(axis) - b.max(axis)) / v.y;
    }
    else if (a.max(axis) > b.min(axis) && v.y > 0) {
        u_1.x = (a.max(axis) - b.min(axis)) / v.y;
    }


    u0 = std::max(u_0.x, u_0.y);
    u1 = std::min(u_1.x, u_1.y);

    if (u0 <= u1) {
        return true;
    }

    return u0 <= u1;
}
