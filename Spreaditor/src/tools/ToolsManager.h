#pragma once

#include "Manager.h"

class ToolsManager : public Manager<ToolsManager> {
	friend class CRSP<ToolsManager>;
private:
	ToolsManager();
	~ToolsManager();
public:
	void start_up() override;
	void shut_down() override;

	float tools_width()const { return m_tools_width; };

	void draw_tools_gui();


private:
	float m_tools_width{ 50.0f };

};

