//
// Created by Adams Humbert on 8/14/2025.
//

#ifndef CLOCK_H
#define CLOCK_H
#include <chrono>

#include <windows.h>                // for Windows APIs


static LARGE_INTEGER ticks_per_second, last_tick_count, current_tick_count;

struct m_clock
{
    bool active = false;
    float delta_time;
    float last_time;
};

//everything by default is in microseconds
#define SECONDS(x) (x/1000.0f); //ex: 5 seconds -> 0.0005 microseconds
#define MINUTES(x) (SECONDS(x)*60.0f); //

//constexpr float SECONDS(float s) { return s / 1000.0f;}

void clock_windows_init();
float clock_window_delta_time();
void clock_window_time(m_clock& clock);



float delta_time_to_second(float dt);
float delta_time_to_fps(float dt);






#endif //CLOCK_H
