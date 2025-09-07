//
// Created by Adams Humbert on 8/17/2025.
//

#ifndef UI_H
#define UI_H

#include "Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//IMMEDIATE MODE UI
struct UI_Push_Constants
{
    //TODO: values are temporary for now
    glm::vec2 screenSize{800.0, 600.0};
};


struct UIID
{
    int ID;
    int layer;
};

//TODO: update to SOA
struct UI_OBJECT
{
    UIID id;
    glm::vec2 position;
    glm::vec2 screen_percentage;
    glm::vec3 color;
};

struct UI_DRAW_INFO
{
    VERTEX_DYNAMIC_INFO vertex_info;
    UI_Push_Constants push_constants;
    std::vector<UI_OBJECT> UI_Objects;
};

struct UI_STATE
{
    UIID hot;
    UIID active;

    int mouse_down = 0;
    int mouse_released = 0;
    glm::vec2 mouse_pos = {-1, -1};

    UI_DRAW_INFO draw_info{};
};

inline UI_STATE* init_ui_state()
{
    UI_STATE* ui_state = static_cast<UI_STATE *>(malloc(sizeof(UI_STATE)));
    ui_state->active.ID = -1;
    ui_state->active.layer = -1;
    ui_state->hot.ID = -1;
    ui_state->hot.layer = -1;
    return ui_state;
}




enum UI_Type
{
    color,
    texture,
};


struct TRANSFORM_2D
{
    glm::vec2 translation{};
    float rotation{};
    glm::vec2 scale{1.0f, 1.0f};

    glm::mat2 mat2()
    {
        float s = glm::sin(rotation);
        float c = glm::cos(rotation);
        glm::mat2 rotMatrix{{scale.x, s}, {c, scale.y}};

        glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
        return rotMatrix * scaleMat;
    }
};


inline glm::vec2 get_mouse_pos(GLFWwindow* window)
{
    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}


struct Font_Texture
{
    glm::vec2 size;
    glm::vec2 sub_size;
};

inline std::vector<Vertex> UI_create_quad(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
{
    return {
        {{pos.x, pos.y}, {color}},
        {{pos.x, pos.y + size.y}, {color}},
        {{pos.x + size.x, pos.y + size.y}, {color}},
        {{pos.x + size.x, pos.y}, {color}}
    };
}

inline std::vector<Vertex> UI_create_quad_screen_percentage(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
{
    return {
        {{pos.x - size.x, pos.y - size.y}, {color}},
        {{pos.x - size.x, pos.y + size.y}, {color}},
        {{pos.x + size.x, pos.y + size.y}, {color}},
        {{pos.x + size.x, pos.y - size.y}, {color}}
    };
}


inline int ui_draw_rect(glm::vec2 pos, glm::vec2 size, glm::vec3 color, UI_STATE& ui_state)
{
    std::vector<Vertex> new_quad = UI_create_quad(pos, size, color);
    uint16_t base_index = static_cast<uint16_t>(ui_state.draw_info.vertex_info.dynamic_vertices.size());

    // Add vertices
    ui_state.draw_info.vertex_info.dynamic_vertices.insert(ui_state.draw_info.vertex_info.dynamic_vertices.end(), new_quad.begin(),
                                                     new_quad.end());

    // Add indices (two triangles per quad)
    std::vector<uint16_t> quad_indices = {
        static_cast<uint16_t>(base_index + 0),
        static_cast<uint16_t>(base_index + 1),
        static_cast<uint16_t>(base_index + 2),
        static_cast<uint16_t>(base_index + 2),
        static_cast<uint16_t>(base_index + 3),
        static_cast<uint16_t>(base_index + 0)
    };

    ui_state.draw_info.vertex_info.dynamic_indices.insert(ui_state.draw_info.vertex_info.dynamic_indices.end(),
                                                    quad_indices.begin(), quad_indices.end());


    ui_state.draw_info.vertex_info.vertex_buffer_should_update = true;

    return ui_state.draw_info.vertex_info.mesh_id++;
};


inline int ui_draw_rect_screen_size_percentage(UI_STATE* ui_state, glm::vec2 pos, glm::vec2 screen_percentage, glm::vec3 color)
{
    //TODO: so rn its based on the topleft corner, but i want to expand the object from the center of where i put it not from top left

    //so in theory, giving a position on where it should be on the screen is fucking terrible, cause it will 100 go off the screen on a resize
    //so a -1,1, device coordinate wouldn't actually be a bad idea, we just specify where relatively on the screen we want it


    //tells us how much of the screen we want based on the size percentage
    if (screen_percentage.x > 100 || screen_percentage.x < 0 || screen_percentage.y > 100 || screen_percentage.y < 0)
    {
        printf("YOU ARE STUPID");
    }
    if (pos.x > 100 || pos.x < 0 || pos.y > 100 || pos.y < 0)
    {
        printf("YOU ARE STUPID 2 ");
    }


    //so we specify what percent on the screen we want to start, and how big the elemenst are
    // 1920 * 1080    we want to position it at at 10% left, 10% down, then 10% wide and height

    //convert 100 -> 1
    glm::vec2 converted_pos = pos / 100.0f;
    glm::vec2 converted_size = screen_percentage / 100.0f;

    //these are the values in our vertex, but not in our ui_object
    glm::vec2 final_pos = {
        ui_state->draw_info.push_constants.screenSize.x * converted_pos.x,
        ui_state->draw_info.push_constants.screenSize.y * converted_pos.y
    };

    glm::vec2 final_size = {
        (ui_state->draw_info.push_constants.screenSize.x * converted_size.x) / 2,
        (ui_state->draw_info.push_constants.screenSize.y * converted_size.y) / 2
    };


    //std::vector<Vertex> new_quad = UI_create_quad(pos, screen_percentage, color);


    //std::vector<Vertex> new_quad = UI_create_quad(pos, screen_percentage, color);
    //this anchors the ui top left
    /*
    std::vector<Vertex> new_quad =  {
             {{final_pos.x , final_pos.y}, {color}},
                {{final_pos.x, final_pos.y + final_size.y}, {color}},
                {{final_pos.x + final_size.x, final_pos.y + final_size.y}, {color}},
                {{final_pos.x + final_size.x, final_pos.y}, {color}}
    };*/
    std::vector<Vertex> new_quad = UI_create_quad_screen_percentage(final_pos, final_size, color);

    std::cout << "QUAD:" << new_quad[0].pos.x << new_quad[0].pos.y << '\n';
    std::cout << "QUAD:" << new_quad[1].pos.x << new_quad[1].pos.y << '\n';
    std::cout << "QUAD:" << new_quad[2].pos.x << new_quad[2].pos.y << '\n';
    std::cout << "QUAD:" << new_quad[3].pos.x << new_quad[3].pos.y << '\n';

    uint16_t base_index = static_cast<uint16_t>(ui_state->draw_info.vertex_info.dynamic_vertices.size());

    // Add vertices
    ui_state->draw_info.vertex_info.dynamic_vertices.insert(ui_state->draw_info.vertex_info.dynamic_vertices.end(), new_quad.begin(),
                                                     new_quad.end());

    // Add indices (two triangles per quad)
    std::vector<uint16_t> quad_indices = {
        static_cast<uint16_t>(base_index + 0),
        static_cast<uint16_t>(base_index + 1),
        static_cast<uint16_t>(base_index + 2),
        static_cast<uint16_t>(base_index + 2),
        static_cast<uint16_t>(base_index + 3),
        static_cast<uint16_t>(base_index + 0)
    };

    ui_state->draw_info.vertex_info.dynamic_indices.insert(ui_state->draw_info.vertex_info.dynamic_indices.end(),
                                                    quad_indices.begin(), quad_indices.end());


    ui_state->draw_info.vertex_info.vertex_buffer_should_update = true;

    UI_OBJECT new_ui_object = {{ui_state->draw_info.vertex_info.mesh_id, 0}, converted_pos, converted_size, color};

    //increment id
    ui_state->draw_info.vertex_info.mesh_id++;

    ui_state->draw_info.UI_Objects.emplace_back(new_ui_object);

    return ui_state->draw_info.vertex_info.mesh_id;
};


inline void update_UI_on_resize(UI_DRAW_INFO& ui_draw_info)
{
    //TODO: figure out how to change the size, maybe reconstruct all the ui's????
    //but also keep all the initial size it was set with - I would have to do this anyway, if say i wanted to change the size at runtime without a window resize

    //iterate over all ui objects and recalculate their size values


    //so in theory, giving a position on where it should be on the screen is fucking terrible, cause it will 100 go off the screen on a resize
    //so a -1,1, device coordinate wouldn't actually be a bad idea, we just specify where relatively on the screen we want it

    ui_draw_info.vertex_info.dynamic_vertices.clear();
    ui_draw_info.vertex_info.dynamic_indices.clear();

    for (auto ui_object: ui_draw_info.UI_Objects)
    {
        //so we specify what percent on the screen we want to start, and how big the elemenst are
        // 1920 * 1080    we want to position it at at 10% left, 10% down, then 10% wide and height


        //these are the values in our vertex, but not in our ui_object
        glm::vec2 final_pos = {
            ui_draw_info.push_constants.screenSize.x * ui_object.position.x,
            ui_draw_info.push_constants.screenSize.y * ui_object.position.y
        };
        glm::vec2 final_size = {
            (ui_draw_info.push_constants.screenSize.x * ui_object.screen_percentage.x) / 2,
            (ui_draw_info.push_constants.screenSize.y * ui_object.screen_percentage.y) / 2
        };


        //std::vector<Vertex> new_quad = UI_create_quad(pos, screen_percentage, color);
        std::vector<Vertex> new_quad = UI_create_quad_screen_percentage(final_pos, final_size, ui_object.color);


        uint16_t base_index = static_cast<uint16_t>(ui_draw_info.vertex_info.dynamic_vertices.size());

        // Add vertices
        ui_draw_info.vertex_info.dynamic_vertices.insert(ui_draw_info.vertex_info.dynamic_vertices.end(),
                                                         new_quad.begin(), new_quad.end());

        // Add indices (two triangles per quad)
        std::vector<uint16_t> quad_indices = {
            static_cast<uint16_t>(base_index + 0),
            static_cast<uint16_t>(base_index + 1),
            static_cast<uint16_t>(base_index + 2),
            static_cast<uint16_t>(base_index + 2),
            static_cast<uint16_t>(base_index + 3),
            static_cast<uint16_t>(base_index + 0)
        };

        ui_draw_info.vertex_info.dynamic_indices.insert(ui_draw_info.vertex_info.dynamic_indices.end(),
                                                        quad_indices.begin(), quad_indices.end());
    }

    ui_draw_info.vertex_info.vertex_buffer_should_update = true;
}


inline bool region_hit(UI_Push_Constants push_constants, UI_OBJECT ui_object, glm::vec2 mouse_pos)
{
    //check if we are inside a ui_object

    //these are the values in our vertex, but not in our ui_object
    glm::vec2 final_pos = {
        push_constants.screenSize.x * ui_object.position.x, push_constants.screenSize.y * ui_object.position.y
    };
    glm::vec2 final_size = {
        (push_constants.screenSize.x * ui_object.screen_percentage.x) / 2,
        (push_constants.screenSize.y * ui_object.screen_percentage.y) / 2
    };
    glm::vec2 pos = final_pos;
    glm::vec2 size = final_size;

    // bottom left
    if (pos.x - size.x > mouse_pos.x) return false;
    if (pos.y - size.y > mouse_pos.y) return false;

    // top left
    if (pos.x - size.x > mouse_pos.x) return false;
    if (pos.y + size.y < mouse_pos.y) return false;

    // top right
    if (pos.x + size.x < mouse_pos.x)return false;
    if (pos.y + size.y < mouse_pos.y) return false;

    // buttom right
    if (pos.x + size.x < mouse_pos.x) return false;
    if (pos.y - size.y > mouse_pos.y) return false;

    return true;
}

/*
bool button(UI_STATE& ui_state, int id, int x, int y)
{
    if (region_hit())
    {
        ui = hot;
        if (acitve == 0 and mouse down)
            activeitem = id
    }
}*/

//check if we can use the button
inline bool do_button(UI_STATE* ui_state, UI_OBJECT ui_object)
{
    //checking if we released the button and we are inside the hit region
    if (ui_state->mouse_down == 0 &&
        ui_state->active.ID == ui_object.id.ID and region_hit(ui_state->draw_info.push_constants, ui_object, ui_state->mouse_pos))
        return true;

    return false;
}

inline void ui_update(UI_STATE* ui_state, GLFWwindow* window)
{
    //TODO: for the future, set the highest layer as the active ui, doesn't matter rn
    //set our UI's states
    for (auto& ui_object: ui_state->draw_info.UI_Objects)
    {
        if (region_hit(ui_state->draw_info.push_constants, ui_object, ui_state->mouse_pos))
        {
            ui_state->hot = ui_object.id;
            //std::cout << "HOT:" << ui_object.id.ID << std::endl;

            if (ui_state->active.ID == -1 && ui_state->mouse_down == GLFW_PRESS)
            {
                ui_state->active = ui_object.id;
                //std::cout << "active:" << ui_object.id.ID << std::endl;
            }
        }
    }


    //todo: this should be moved to the do button function
    //setting any colors
    for (auto& ui_object: ui_state->draw_info.UI_Objects)
    {
        /*
        std::cout << "hot id: " << ui_state.hot.ID << '\n';
        std::cout << "active id: " <<  ui_state.active.ID<< '\n';
        */

        //active state
        if (ui_state->active.ID == ui_object.id.ID)
        {
            for (int i = ui_object.id.ID * 4; i < ui_object.id.ID + 4; i++)
            {
                ui_state->draw_info.vertex_info.dynamic_vertices[i].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            }
        }
        //hot state
        else if (ui_state->hot.ID == ui_object.id.ID)
        {
            for (int i = ui_object.id.ID * 4; i < ui_object.id.ID + 4; i++)
            {
                ui_state->draw_info.vertex_info.dynamic_vertices[i].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            }
        }
        // normal state
        else
        {
            for (int i = ui_object.id.ID * 4; i < ui_object.id.ID + 4; i++)
            {
                ui_state->draw_info.vertex_info.dynamic_vertices[i].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
        }

        if (do_button(ui_state, ui_object))
        {
            std::cout << "Click\n";
        };
    }


    ui_state->draw_info.vertex_info.vertex_buffer_should_update = true;
    //std::cout << "MOUSE STATE:" << ui_state.mouse_down << '\n';

    ui_state->hot.ID = -1;
    ui_state->hot.layer = -1;

    if (ui_state->mouse_down == GLFW_RELEASE)
    {
        //std::cout << "mouse released\n";
        ui_state->active.ID = -1;
    }

    //update mouse state
    ui_state->mouse_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    //update mouse pos
    ui_state->mouse_pos = get_mouse_pos(window);
}



inline void do_text()
{



}





#endif //UI_H
