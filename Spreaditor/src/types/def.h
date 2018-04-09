#pragma once

#include <utility>
#include <unordered_set>
#include <set>


using byte = unsigned char;
using EntityId = unsigned int;
using EntityIdSet = std::set<EntityId>;
using Chunk = std::pair<int, int>;

struct PairIntIntHash {
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        auto hash = std::size_t{ gsl::narrow_cast<size_t>(x.first) };
        hash ^= x.second + 0x9e3779b9 + (x.first << 6) + (x.first >> 2);
        return hash;
    }
};

using ChunkSet = std::unordered_set<Chunk, PairIntIntHash>;
