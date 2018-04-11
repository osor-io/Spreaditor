#pragma once

#include <utility>

using byte = unsigned char;

struct PairIntIntHash {
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        auto hash = std::size_t{ gsl::narrow_cast<size_t>(x.first) };
        hash ^= x.second + 0x9e3779b9 + (x.first << 6) + (x.first >> 2);
        return hash;
    }
};

