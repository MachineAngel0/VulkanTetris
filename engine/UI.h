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



void ui_draw_rect(glm::vec2 pos)
{
};






#endif //UI_H
