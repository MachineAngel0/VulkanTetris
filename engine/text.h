//
// Created by Adams Humbert on 9/10/2025.
//

#ifndef TEXT_H
#define TEXT_H


#include <glm/glm.hpp>
#include "stb_truetype.h"


struct Vertex_Text
{
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 tex;
};


struct Text_System
{

};

void text_system_init()
{

};

void load_font(const char* filepath)
{

};


#endif //TEXT_H
