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

	void tick();
	void draw_tools_gui();


private:

	Tool m_current_tool = Tool::NONE;

	bool m_dragging{ false };


	//// Collider Creation
	bool m_currently_creating_collider{ false };
	ColliderRect m_new_collider{};
	ImColor m_new_collider_color{ 255,0,0,130 };


	float m_tools_width{ 50.0f };

};

