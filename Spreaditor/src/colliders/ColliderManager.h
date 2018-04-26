#pragma once

#include <Manager.h>
#include "Colliders.h"
#include "ColliderFactory.h"
#include <set>
#include <map>
#include <utility>
#include "json.hpp"

#define ALPHA_LIMIT 0.6

struct RectToDelete {
	std::string type_name;
	std::string instance_name;
	int sprite_index;
	int rect_index;
};

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
	json colliders_to_json(bool ignore_rects = false) const;
	bool colliders_from_json(const json& data, bool ignore_rects = false);
	float timeline_width()const { return m_timeline_width; }

	void set_selected_window_name(const std::string& name) { m_selected_window_name = name; }

	void request_rect_to_delete(const RectToDelete& rect);

	void update();

	/**
	Removes all rects from all the instances and colliders.

	Mainly used when loading a new set of sprites and we want to
	keep the types and instances intact but the rects defined no longer
	make sense to keep since the sprites have changed.
	*/
	void clear_rects();

	void draw_collider_gui();

private:
	ColliderContainer m_colliders;
	float m_timeline_width{ 200.f };

	std::string m_selected_window_name{};

	std::vector<RectToDelete> m_rects_to_delete{};

};

