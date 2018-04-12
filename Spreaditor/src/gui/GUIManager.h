#pragma once

#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>


// Both of these multiply ImGui's font size to calculate the real size.
#define TIMELINE_SIZE 10
#define TIMELINE_HEADER_SIZE 1

class GUIManager : public Manager<GUIManager> {
    friend class CRSP<GUIManager>;
private:
    GUIManager();
    ~GUIManager();

public:

    void start_up() override;
    void shut_down() override;


    void set_debug_open(bool);
    bool is_debug_open() const;

    void do_gui();
    void draw_corner_overlay_debug_info();
    void draw_timeline();
    void draw_timeline_sprite(const sf::Sprite& sprite, int sprite_index, float height);




private:
    ImFont * m_font;
    bool m_debug_open{ true };


    bool m_show_imgui_demo{ false };
    bool m_show_debug_overlay{ true };


    bool m_show_timeline{ true };

};
