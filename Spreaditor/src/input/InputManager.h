#pragma once
#include <Manager.h>
#include "SFML/Window.hpp"

class InputManager : public Manager<InputManager> {
    friend class CRSP<InputManager>;
private:
    InputManager();
    ~InputManager();

public:

    void start_up() override;
    void shut_down() override;

    static bool is_key_pressed(sf::Keyboard::Key key);
    static bool is_button_pressed(unsigned int button);
    static float get_axis_position(sf::Joystick::Axis axis);

private:


};
