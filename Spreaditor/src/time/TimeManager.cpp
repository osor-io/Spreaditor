#include "TimeManager.h"

TimeManager::TimeManager() {}


TimeManager::~TimeManager() {}


void TimeManager::start_up() {
    m_frame_clock = std::make_unique<sf::Clock>();
    m_frame_clock->restart();

    m_execution_clock = std::make_unique<sf::Clock>();
    m_execution_clock->restart();

    auto frequency = LARGE_INTEGER{};

    QueryPerformanceFrequency(&frequency);
    m_internal_frequency = frequency.QuadPart;
}


void TimeManager::shut_down() {
    m_frame_clock.reset();
}


sf::Time TimeManager::end_frame() {
    m_frame_delta_time = m_frame_clock->restart();
    return m_frame_delta_time;
}

sf::Time TimeManager::get_delta_time() const {
    return m_frame_delta_time;
}

sf::Time TimeManager::get_execution_time() const {
    return m_execution_clock->getElapsedTime();
}

void TimeManager::begin_tick() {
    m_beg_of_tick = query_cycle_counter();
}

float TimeManager::end_tick() {
    m_end_of_tick = query_cycle_counter();
    m_tick_clocks = m_end_of_tick - m_beg_of_tick;
    m_tick_ms = cycles_to_ms(m_tick_clocks);
    return m_tick_ms;
}

float TimeManager::get_tick_ms() const {
    return m_tick_ms;
}

long long TimeManager::query_cycle_counter() {
    auto counter = LARGE_INTEGER{};
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

float TimeManager::cycles_to_ms(CounterType counter) {
    return (1000 * counter) / m_internal_frequency;
}


