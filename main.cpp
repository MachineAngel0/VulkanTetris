#include <iostream>

#include "clock.h"
#include "Tetris.h"
#include "Renderer.h"


int main()
{

    Vulkan_Context vulkan_context{};
    GLFW_Window_Context window_info{};
    Swapchain_Context swapchain_context{};
    Graphics_Context graphics_context{};
    Command_Buffer_Context command_buffer_context{};
    Semaphore_Fences_Context semaphore_fences_context{};

    // reserve space for the vertice data
    VERTEX_DYNAMIC_INFO vertex_info{}; //TODO: move this into the game state, since a lot of things will want access to this
    vertex_info.dynamic_vertices.reserve(MAX_VERTICES);
    vertex_info.dynamic_vertices.reserve(MAX_INDICES);

    Game_State* game_state = (Game_State*)malloc(sizeof(Game_State));

    game_state->tetris_grid = create_grid(vertex_info, GRID_COLUMN, GRID_ROW);
    //game_state->current_block = create_block(vertex_info, O);
    game_state->current_tetromino = pick_new_tetromino(vertex_info);

    Tetris_Clock tetris_clock;
    game_state->tetris_clock = tetris_clock;
    tetris_clock_init(game_state->tetris_clock);




    init_vulkan(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
                    semaphore_fences_context);

    clock_windows_init();


    float dt = 0.0f; // in ms
    while (!glfwWindowShouldClose(window_info.window))
    {

        dt = clock_window_delta_time();
        //printf("%fMS\n", dt);
        //printf("%fFPS\n", delta_time_to_fps(dt));

        glfwPollEvents();
        key_callback(window_info.window, game_state, vertex_info);

        update_game(game_state, vertex_info, dt);

        draw_frame(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
                   semaphore_fences_context, vertex_info);
    }

    vkDeviceWaitIdle(vulkan_context.logical_device);




    cleanup(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
                semaphore_fences_context);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}


