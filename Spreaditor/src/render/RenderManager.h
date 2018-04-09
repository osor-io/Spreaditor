#pragma once
#include <Manager.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>


class RenderManager : public Manager<RenderManager> {
    friend class CRSP<RenderManager>;
private:
    RenderManager();
    ~RenderManager();

public:

    void start_up() override;
    void shut_down() override;

    sf::RenderTarget* get_main_render_target();

    void set_main_target_params(sf::Vector2f center, float zoom);

    sf::Vector2f get_current_center() const;
    float get_current_zoom() const;

    void begin_frame();
    void end_frame();

    sf::Vector2i map_coords_to_pixel(sf::Vector2f position, bool scaled = true);

private:

    sf::Color m_clear_color{ sf::Color::Black };
    sf::RenderWindow* m_main_target;

    sf::Vector2f m_current_center{ 0.0f,0.0f };
    float m_current_zoom{ 1.0f };


};
