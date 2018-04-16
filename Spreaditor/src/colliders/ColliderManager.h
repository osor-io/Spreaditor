#pragma once

#include <Manager.h>
#include "Colliders.h"
#include "ColliderFactory.h"
#include <unordered_map>
#include <unordered_set>


class ColliderManager : public Manager<ColliderManager> {
    friend class CRSP<ColliderManager>;
    using InstanceContainer = std::unordered_set<ColliderInstance>;
    using ColliderContainer = std::unordered_map<ColliderType, InstanceContainer>;
private:
    ColliderManager();
    ~ColliderManager();
public:
    void start_up() override;
    void shut_down() override;

    const ColliderContainer& get_colliders() const { return m_colliders; }

    void draw_collider_gui();

private:
    ColliderContainer m_colliders;

};

