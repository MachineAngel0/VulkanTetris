#include <iostream>
#include <queue>

#include "clock.h"
#include "Tetris.h"
#include "Renderer.h"
#include "UI.h"


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


    Graphics_Context ui_graphics_context{};
    Command_Buffer_Context ui_command_buffer_context{};
    UI_DRAW_INFO ui_draw_info{};
    ui_draw_info.push_constants.screenSize = {glm::vec2(600.0f,600.0f)};
    //ui_draw_rect(glm::vec2{100.0,100.0}, glm::vec3{0.0,1.0,0.0}, ui_draw_info);
    ui_draw_rect(glm::vec2{0.0f,0.0f}, glm::vec2{300.0f, 300.0f}, glm::vec3{0.0,1.0,0.0}, ui_draw_info);


    init_vulkan(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
                    semaphore_fences_context);
    init_UI_vulkan(vulkan_context, swapchain_context, ui_graphics_context, ui_command_buffer_context);
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
                   semaphore_fences_context, vertex_info, ui_graphics_context, ui_command_buffer_context, ui_draw_info);
    }

    vkDeviceWaitIdle(vulkan_context.logical_device);




    cleanup(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
                semaphore_fences_context);

    std::cout << "Bye, World!" << std::endl;


    return 0;
}


