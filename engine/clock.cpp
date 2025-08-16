//
// Created by Adams Humbert on 8/14/2025.
//

#include "clock.h"

void clock_windows_init()
{
    QueryPerformanceCounter(&ticks_per_second);
    QueryPerformanceCounter(&last_tick_count);
}


void clock_window_time(m_clock& clock)
{
    //get current tick
    QueryPerformanceCounter(&current_tick_count);
    //difference from last frame
    double elapsed_ticks = current_tick_count.QuadPart - last_tick_count.QuadPart;
    // to milliseconds
    double temp_delta = (elapsed_ticks*100) / ticks_per_second.QuadPart;
    last_tick_count = current_tick_count;

    temp_delta = float(temp_delta);

    clock.delta_time = temp_delta;
    clock.last_time = temp_delta;

}

float clock_window_delta_time()
{
    //get current tick
    QueryPerformanceCounter(&current_tick_count);
    //difference from last frame
    double elapsed_ticks = current_tick_count.QuadPart - last_tick_count.QuadPart;
    // to milliseconds
    double temp_delta = (elapsed_ticks*100) / ticks_per_second.QuadPart;

    //store last frame time
    last_tick_count = current_tick_count;

    //do a conversion
    return (float) temp_delta;
}

float delta_time_to_second(float dt)
{
    return dt/1000.0f;
}

float delta_time_countdown_timer(float dt)
{
    return dt*1000.0f;
}
float delta_time_to_fps(float dt)
{
    return 1000.0f/dt;
}



