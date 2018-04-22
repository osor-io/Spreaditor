#pragma once

#include "Manager.h"
#include "Tools.h"
#include "../colliders/Colliders.h"
#include <imgui.h>
#include <imgui-SFML.h>

class ToolsManager : public Manager<ToolsManager> {
	friend class CRSP<ToolsManager>;
private:
	ToolsManager();
	~ToolsManager();
public:
	void start_up() override;
	void shut_down() override;

	float tools_width()const { return m_tools_width; };
	float tools_options_bar_height() const { return m_tools_options_bar_height; }

	void tick();
	void draw_tools_gui();


private:

	void draw_rects_of_instance(const ColliderInstance & instance, int current_sprite_index, ImDrawList * draw_list);


	Tool m_current_tool = Tool::NONE;

	bool m_dragging{ false };

	//// Types & Instances
	ColliderType* m_p_selected_type{ nullptr };
	ColliderType m_selected_type_copy{};
	ColliderInstance* m_p_selected_instance{ nullptr };
	ColliderInstance m_selected_instance_copy{};


	//// Collider Creation
	bool m_currently_creating_collider{ false };
	ColliderRect m_new_collider{};
	ImColor m_new_collider_color{ 255,0,0,130 };


	//// Collider Rect Rendering
	bool m_show_all_rects{ false };

	float m_tools_width{ 50.0f };
	float m_tools_options_bar_height{ 20.0f };

};

