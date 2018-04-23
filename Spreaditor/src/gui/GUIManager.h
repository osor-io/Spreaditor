#pragma once

#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <json.hpp>


// Both of these multiply ImGui's font size to calculate the real size.
#define TIMELINE_SIZE 10
#define TIMELINE_HEADER_SIZE 1

#define CHECK_FILE_INTERVAL 100 //ms

namespace config {

    const auto style_filename = "style.json";

}

class GUIManager : public Manager<GUIManager> {
    using json = nlohmann::json;
    friend class CRSP<GUIManager>;
private:
    GUIManager();
    ~GUIManager();

public:

    void start_up() override;
    void shut_down() override;


    void set_debug_open(bool);
    bool is_debug_open() const;
    float get_main_menu_height() const { return m_main_menu_height; }
    float get_timeline_height() const { return m_timeline_height; }
    const sf::Vector2f& get_global_mouse_pos() const { return m_global_mouse_pos; }
    const sf::Vector2f& get_sprite_mouse_pos() const { return m_sprite_mouse_position; }
    bool is_mouse_inside_sprite() const { return m_is_mouse_inside_sprite; }
	float scaling() const { return m_scaling_factor; }
	sf::Vector2f sprite_to_global(sf::Vector2f) const;
	sf::Vector2f global_to_sprite(sf::Vector2f) const;


    void update();
    void do_gui();
    void draw_corner_overlay_debug_info();
    void draw_timeline();
    void draw_style_editor(ImGuiStyle* ref = nullptr);


private:

    void draw_timeline_sprite(const sf::Sprite& sprite, int sprite_index, float height);

    json style_to_json();
    void style_from_json(json json_style);

    ImFont * m_font;
    bool m_debug_open{ true };


    bool m_show_imgui_demo{ false };
    bool m_show_debug_overlay{ true };


    bool m_show_timeline{ true };
    bool m_show_collider_explorer{ true };
	bool m_show_tools{ true };
    bool m_show_style_editor{ false };

    float m_main_menu_height{ 20.f };
    float m_timeline_height{ 200.0f };

    sf::Vector2f m_global_mouse_pos{};
    sf::Vector2f m_sprite_mouse_position{};
    bool m_is_mouse_inside_sprite{ false };

    float m_scaling_factor{ 1.0f };

};
