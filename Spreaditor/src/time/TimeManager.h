#pragma once
#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>

//@@NOTE  @@PLATTFORM
#include <windows.h>


#define DELTA_TIME TimeManager::get().get_delta_time()
#define DELTA_SECONDS TimeManager::get().get_delta_time().asSeconds()
#define DELTA_MILLISECONDS TimeManager::get().get_delta_time().asMilliseconds();
#define DELTA_MICROSECONDS TimeManager::get().get_delta_time().asMicroseconds();


using CounterType = long long;

class TimeManager : public Manager<TimeManager> {
    friend class CRSP<TimeManager>;
private:
    TimeManager();
    ~TimeManager();

public:

    void start_up() override;
    void shut_down() override;

    sf::Time end_frame();
    sf::Time get_delta_time() const ;

    void begin_tick();
    float end_tick();
    float get_tick_ms() const;

    CounterType query_cycle_counter();

    float cycles_to_ms(CounterType counter);

private:
    std::unique_ptr<sf::Clock> m_frame_clock;
    sf::Time m_frame_delta_time{};
    CounterType m_internal_frequency{};

    CounterType m_beg_of_tick{};
    CounterType m_end_of_tick{};
    CounterType m_tick_clocks{};
    float m_tick_ms{};
};
