#pragma once

#include <Manager.h>
#include "Colliders.h"
#include "ColliderFactory.h"
#include <set>
#include <map>
#include <utility>

#define ALPHA_LIMIT 0.6

class ColliderManager : public Manager<ColliderManager> {
	friend class CRSP<ColliderManager>;
	using InstanceContainer = std::set<ColliderInstance>;
	using ColliderContainer = std::map<ColliderType, InstanceContainer>;
private:
	ColliderManager();
	~ColliderManager();
public:
	void start_up() override;
	void shut_down() override;

	const ColliderContainer& get_colliders() const { return m_colliders; }
	bool write_colliders_to_file(const char* filename) const;

	void draw_collider_gui();

private:
	ColliderContainer m_colliders;

};

