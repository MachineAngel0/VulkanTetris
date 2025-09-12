//
// Created by Adams Humbert on 9/10/2025.
//

#ifndef TEXT_H
#define TEXT_H

#include <vulkan/vulkan.h>
#include <array>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "stb_truetype.h"
#include "texture.h"


struct Vulkan_Context;
struct Command_Buffer_Context;
struct Buffer_Context;

struct Vertex_Text
{
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 tex;
};

inline std::vector<Vertex_Text> text_verticies = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

VkVertexInputBindingDescription get_text_binding_description();

std::array<VkVertexInputAttributeDescription, 3> get_text_attribute_descriptions();


struct Glyph {
    int width;
    int height;
    int xoff;
    int yoff;
    float advance;
    unsigned char* bitmap; // 8-bit alpha bitmap
};

struct Text_System
{
    stbtt_fontinfo font;
    Glyph glyphs[96];
    Texture glyph_textures[96];


    std::vector<Vertex_Text> dynamic_vertices{};
    std::vector<uint16_t> dynamic_indices{};

};

void text_system_init();

bool load_font(Text_System& text_system, const char* filepath);;

//TODO: load into an atlus
bool load_font_into_atlas(Text_System& text_system, const char* filepath);


std::vector<Vertex_Text> text_create_quad(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 tex);

void do_text(Text_System& text_system, std::string text);


void text_vertex_buffer_update(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, Buffer_Context& buffer_context, Text_System& text_system);


void text_update(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, Buffer_Context& buffer_context, Text_System& text_system);

void create_text_vertex_buffer_new(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, Buffer_Context& buffer_context);



#endif //TEXT_H
