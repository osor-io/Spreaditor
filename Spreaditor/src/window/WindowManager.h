#pragma once
#include <Manager.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>

class WindowManager : public Manager<WindowManager> {
    friend class CRSP<WindowManager>;
public:
    WindowManager();
    ~WindowManager();


    void start_up() override;
    void shut_down() override;

    void fill_events();

    sf::RenderWindow* get_window_render_target();
    bool is_window_open() const;
    const std::vector<sf::Event>& get_frame_events() const;

    sf::WindowHandle & get_window_handle();

private:
    std::vector<sf::Event> m_frame_events;
    std::unique_ptr<sf::RenderWindow> m_window;

};

