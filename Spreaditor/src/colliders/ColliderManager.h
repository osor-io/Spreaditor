#pragma once

#include <Manager.h>
#include "Colliders.h"
#include "ColliderFactory.h"
#include <set>
#include <map>
#include <utility>
#include "json.hpp"

#define ALPHA_LIMIT 0.6

class ColliderManager : public Manager<ColliderManager> {
	friend class CRSP<ColliderManager>;
	using InstanceContainer = std::set<ColliderInstance>;
	using ColliderContainer = std::map<ColliderType, InstanceContainer>;
	using json = nlohmann::json;
private:
	ColliderManager();
	~ColliderManager();
public:
	void start_up() override;
	void shut_down() override;

	const ColliderContainer& get_colliders() const { return m_colliders; }
	ColliderContainer& get_non_const_colliders() { return m_colliders; }
	bool write_colliders_to_file(const char* filename) const;
	json colliders_to_json() const;
	bool colliders_from_json(const json& data);
	float timeline_width()const { return m_timeline_width; }

	void draw_collider_gui();

private:
	ColliderContainer m_colliders;
	float m_timeline_width{ 200.f };

};

