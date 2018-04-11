#pragma once

#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>


class GUIManager : public Manager<GUIManager> {
    friend class CRSP<GUIManager>;
private:
    GUIManager();
    ~GUIManager();

public:

    void start_up() override;
    void shut_down() override;


    void do_gui();
    void draw_corner_overlay_debug_info();

    void set_debug_open(bool);
    bool is_debug_open() const;


private:
    ImFont * m_font;
    bool m_debug_open{ true };


    bool m_show_imgui_demo{ false };
    bool m_show_debug_overlay{ true };
};
