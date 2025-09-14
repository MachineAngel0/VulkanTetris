#include <iostream>

#include "clock.h"
#include "Tetris.h"
#include "Renderer.h"
#include "text.h"
#include "UI.h"
#include "vk_buffer.h"
#include "vk_command_buffer.h"
#include "vk_descriptor.h"
#include "vk_device.h"


int main()
{


    Vulkan_Context vulkan_context{};
    GLFW_Window_Context window_info{};
    Swapchain_Context swapchain_context{};
    Graphics_Context graphics_context{};
    Command_Buffer_Context command_buffer_context{};
    Buffer_Context buffer_context{};
    Semaphore_Fences_Context semaphore_fences_context{};

    // reserve space for the vertice data
    VERTEX_DYNAMIC_INFO vertex_info{}; //TODO: move this into the game state, since a lot of things will want access to this
    vertex_info.dynamic_vertices.reserve(MAX_VERTICES);
    vertex_info.dynamic_vertices.reserve(MAX_INDICES);

    Game_State* game_state = init_game_state();//static_cast<Game_State *>(malloc(sizeof(Game_State)));
    //TODO: emplace into start game when i finally figure out how to render text

    Graphics_Context ui_graphics_context{};
    Buffer_Context ui_buffer_context{};
    UI_STATE* ui_state = init_ui_state(); // this will cause a crash if moved below the first init



    init_vulkan(vulkan_context, window_info, swapchain_context, graphics_context, buffer_context,
                command_buffer_context, semaphore_fences_context);
    init_UI_vulkan(vulkan_context, swapchain_context, ui_graphics_context, graphics_context, command_buffer_context, ui_buffer_context, ui_state);

    //this has to be here because the ui state won't have the window size until vulkan init's with the window
    ui_draw_button_rect_screen_size_percentage(ui_state, glm::vec2{50,50}, glm::vec2{20,20}, glm::vec3{1.0,0.0,0.0}, glm::vec3{0.0,0.0,1.0}, glm::vec3{0.0,1.0,0.0});

    //TODO: rn text_system is not a pointer, so this is fine
    if (!load_font(ui_state->text_system, "c:/windows/fonts/arialbd.ttf", vulkan_context, command_buffer_context))
    {
        throw std::runtime_error("Failed to load font");
    };


    Graphics_Context text_graphics_context{};
    Buffer_Context text_buffer_context{};
    Descriptor text_descriptor{};
    init_Text_vulkan(vulkan_context, swapchain_context, text_graphics_context, graphics_context, command_buffer_context, text_buffer_context, ui_state->text_system, text_descriptor);
    clock_windows_init();

    float dt = 0.0f; // in ms
    while (!glfwWindowShouldClose(window_info.window))
    {

        dt = clock_window_delta_time();
        //printf("%fMS\n", dt);
        //printf("%fFPS\n", delta_time_to_fps(dt));

        glfwPollEvents();
        key_callback(window_info.window, game_state, vertex_info);

        ui_begin(ui_state);
        /*NEWER TEXT*/
        /*
        if (do_button_new(ui_state, UIID{0, 0}, glm::vec2{25,25}, glm::vec2{49,49}, Alignment::LEFT, glm::vec3{1.0,0.0,0.0}, glm::vec3{0.0,0.0,1.0}, glm::vec3{0.0,1.0,0.0})){
            printf("click\n");
        }
        if (do_button_new_text(ui_state, UIID{0, 0}, glm::vec2{25,25}, glm::vec2{49,49}, "BAHAHAHA", Alignment::LEFT, glm::vec3{1.0,0.0,0.0}, glm::vec3{0.0,0.0,1.0}, glm::vec3{0.0,1.0,0.0})){
            printf("click\n");
        }
        */

        game_update_DOD(game_state, ui_state, vertex_info, dt);


        draw_frame(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
                   buffer_context, vertex_info, semaphore_fences_context, ui_graphics_context, ui_buffer_context, ui_state,
                   text_graphics_context, text_buffer_context, text_descriptor);

        ui_end(ui_state, window_info.window);
    }

    vkDeviceWaitIdle(vulkan_context.logical_device);




    cleanup(vulkan_context, window_info, swapchain_context, graphics_context, command_buffer_context,
            buffer_context, semaphore_fences_context);

    std::cout << "Bye, World!" << std::endl;


    return 0;
}


