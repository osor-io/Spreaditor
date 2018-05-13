#include "WindowManager.h"
#include "../config/Config.h"
#include "../time/TimeManager.h"
#include "../tools/ToolsManager.h" // Using this one could be dangerous: Used to ask for a resize (sets a bool to true) so we can be sure that use is safe
#include "../os/OSStatic.h"
#include "../sprites/SpriteManager.h"


WindowManager::WindowManager() {}

WindowManager::~WindowManager() {}

void WindowManager::start_up() {

	const auto desktop_resolution = get_desktop_resolution();

	const auto horizontal_initial_size_normalized = 0.8f;
	const auto vertical_initial_size_normalized = 0.8f;

	m_window = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(
			desktop_resolution.first * horizontal_initial_size_normalized,
			desktop_resolution.second * vertical_initial_size_normalized),
		config::project_name,
		sf::Style::Default);

	m_window->setVerticalSyncEnabled(true);
}

void WindowManager::shut_down() {
	m_window.reset();
}

void WindowManager::fill_events() {
	auto event = sf::Event{};
	m_frame_events.clear();
	while (m_window->pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);
		m_frame_events.push_back(event);
		if (event.type == sf::Event::Closed) {
			m_window->close();
			return;
		}
	}
}

void WindowManager::manage_events() {

	m_resized_this_frame = false;


	for (const auto& event : m_frame_events) {


		if (event.type == sf::Event::EventType::Resized) {

			if (m_window)
				m_window->setView(sf::View(sf::FloatRect(0.f, 0.f,
					static_cast<float>(m_window->getSize().x),
					static_cast<float>(m_window->getSize().y))));


			m_resized_this_frame = true;

			ToolsManager::get().reread_rect_positions();

		}


		/*
		@@TODO @@MAYBE

		Consider moving the code that deals with inputs of characters
		to a different file that deals only with these hotkeys

		*/

		if (event.type == sf::Event::EventType::KeyPressed) {

			if (!ImGui::GetIO().WantCaptureKeyboard) {

				if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
					SpriteManager::get().go_to_previous_sprite();
				}
				else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
					SpriteManager::get().go_to_next_sprite();
				}

			}

		}

	}

}

sf::RenderWindow * WindowManager::get_window_render_target() {
	return m_window.get();
}

bool WindowManager::is_window_open() const {
	return m_window->isOpen();
}

const std::vector<sf::Event>& WindowManager::get_frame_events() const {
	return m_frame_events;
}

sf::WindowHandle WindowManager::get_window_handle() {
	return m_window->getSystemHandle();
}