#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::start_up() {
}


void InputManager::shut_down() {

}

bool InputManager::is_key_pressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::is_button_pressed(unsigned int button) {

    /*
    We are currently checking in all connected joysticks
    */
    for (auto i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i) && sf::Joystick::isButtonPressed(i, button))
            return true;
    }

    return false;

}

float InputManager::get_axis_position(sf::Joystick::Axis axis) {

    /*
    We are currently checking in all connected joysticks
    */
    for (auto i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            return sf::Joystick::getAxisPosition(i, axis);
        }
    }

    return .0f;
}
