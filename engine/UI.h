//
// Created by Adams Humbert on 8/17/2025.
//

#ifndef UI_H
#define UI_H
#include "Renderer.h"

//IMMEDIATE MODE UI

struct UI_DRAW_INFO
{
    VERTEX_DYNAMIC_INFO vertex_info;
    UI_Push_Constants push_constants;
};
enum UI_Type
{
    color,
    texture,
};


struct TRANSFORM_2D
{
    glm::vec2 translation{};
    float rotation{};
    glm::vec2 scale{1.0f,1.0f};

    glm::mat2 mat2(){
        float s = glm::sin(rotation);
        float c = glm::cos(rotation);
        glm::mat2 rotMatrix {{scale.x, s}, {c, scale.y}};

        glm::mat2 scaleMat {{scale.x, 0.0f}, {0.0f, scale.y}};
        return rotMatrix * scaleMat;
    }
};


struct UI
{
    int id;
    int hot;
    int active;
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

inline int ui_draw_rect(glm::vec2 pos, glm::vec2 size, glm::vec3 color, UI_DRAW_INFO& ui_draw_info)
{
    std::vector<Vertex> new_quad = UI_create_quad(pos, size, color);
    uint16_t base_index = static_cast<uint16_t>(ui_draw_info.vertex_info.dynamic_vertices.size());

    // Add vertices
    ui_draw_info.vertex_info.dynamic_vertices.insert(ui_draw_info.vertex_info.dynamic_vertices.end(), new_quad.begin(), new_quad.end());

    // Add indices (two triangles per quad)
    std::vector<uint16_t> quad_indices = {
        static_cast<uint16_t>(base_index + 0),
        static_cast<uint16_t>(base_index + 1),
        static_cast<uint16_t>(base_index + 2),
        static_cast<uint16_t>(base_index + 2),
        static_cast<uint16_t>(base_index + 3),
        static_cast<uint16_t>(base_index + 0)
    };

    ui_draw_info.vertex_info.dynamic_indices.insert(ui_draw_info.vertex_info.dynamic_indices.end(), quad_indices.begin(), quad_indices.end());


    ui_draw_info.vertex_info.vertex_buffer_should_update = true;

    return ui_draw_info.vertex_info.mesh_id++;

};






#endif //UI_H
