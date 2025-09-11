//
// Created by Adams Humbert on 9/10/2025.
//

#include "text.h"

#include <cstdio>
#include <iostream>

#include "Renderer.h"
#include "vk_vertex.h"
#include "vk_buffer.h"
#include "vk_device.h"

VkVertexInputBindingDescription get_text_binding_description()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex_Text);
    /*
    * VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex
    VK_VERTEX_INPUT_RATE_INSTANCE: Move to the next data entry after each instance
     */
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> get_text_attribute_descriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    //position
    attributeDescriptions[0].binding = 0; //referencing which VkVertexInputBindingDescription binding we are using
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex_Text, pos); //offsetof is pretty interesting

    //color
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex_Text, color);

    //texture
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex_Text, tex);

    return attributeDescriptions;
}

std::vector<Vertex_Text> text_create_quad(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 tex)
{
    return {
        {{pos.x - size.x, pos.y - size.y}, {color}},
        {{pos.x - size.x, pos.y + size.y}, {color}},
        {{pos.x + size.x, pos.y + size.y}, {color}},
        {{pos.x + size.x, pos.y - size.y}, {color}}
    };
}

void do_text(Text_System& text_system, std::string text)
{

    //std::vector<Vertex_Text> new_quad = text_create_quad({0.5f,0.5f}, {0.1f,0.1f}, {1.0f,1.0f,1.0f});

    float x = 0.0f; // cursor position
    float y = 0.0f;

    for (char c : text)
    {
        Glyph& g = text_system.glyphs[c - 32];
        Texture& tex = text_system.glyph_textures[c - 32];

        // Quad position in screen coords
        float xpos = x + g.xoff;
        float ypos = y - g.yoff;
        float w = (float)g.width;
        float h = (float)g.height;

        // UVs cover the whole glyph texture [0,1]
        glm::vec2 uv0(0.0f, 0.0f);
        glm::vec2 uv1(1.0f, 1.0f);

        std::vector<Vertex_Text> new_quad = {
            {{xpos,     ypos},     {1.0f,1.0f,1.0f}, {uv0.x, uv0.y}},
            {{xpos,     ypos + h}, {1.0f,1.0f,1.0f}, {uv0.x, uv1.y}},
            {{xpos + w, ypos + h}, {1.0f,1.0f,1.0f}, {uv1.x, uv1.y}},
            {{xpos + w, ypos},     {1.0f,1.0f,1.0f}, {uv1.x, uv0.y}},
        };

        /*
        std::cout << "QUAD:" << new_quad[0].pos.x << new_quad[0].pos.y << '\n';
        std::cout << "QUAD:" << new_quad[1].pos.x << new_quad[1].pos.y << '\n';
        std::cout << "QUAD:" << new_quad[2].pos.x << new_quad[2].pos.y << '\n';
        std::cout << "QUAD:" << new_quad[3].pos.x << new_quad[3].pos.y << '\n';
        */

        uint16_t base_index = static_cast<uint16_t>(text_system.dynamic_vertices.size());


        // create indices (two triangles per quad)
        std::vector<uint16_t> quad_indices = {
            static_cast<uint16_t>(base_index + 0),
            static_cast<uint16_t>(base_index + 1),
            static_cast<uint16_t>(base_index + 2),
            static_cast<uint16_t>(base_index + 2),
            static_cast<uint16_t>(base_index + 3),
            static_cast<uint16_t>(base_index + 0)
        };

        // Add vertices
        text_system.dynamic_vertices.insert(text_system.dynamic_vertices.end(), new_quad.begin(),
                                            new_quad.end());

        // Add indices
        text_system.dynamic_indices.insert(text_system.dynamic_indices.end(),
                                           quad_indices.begin(), quad_indices.end());

        x += g.advance; // move cursor forward
    }

}

void create_text_vertex_buffer_new(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context,
    Buffer_Context& buffer_context)
{

     uint32_t max_object_count_t = 1000000;
     uint32_t vertices_per_object_t = 4;
    uint32_t MAX_VERTICES_text = max_object_count_t * vertices_per_object_t;

    // Create buffer large enough for maximum vertices, not just initial vertices
    buffer_context.vertex_buffer_capacity = sizeof(Vertex_Text) * MAX_VERTICES_text; // Use MAX_VERTICES instead of vertices.size()

    buffer_create(vulkan_context, buffer_context.vertex_buffer_capacity, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  buffer_context.vertex_staging_buffer, buffer_context.vertex_staging_buffer_memory);

    // Only copy initial vertices data, but allocate full buffer
    VkDeviceSize initial_data_size = sizeof(text_verticies[0]) * vertices.size();

    vkMapMemory(vulkan_context.logical_device, buffer_context.vertex_staging_buffer_memory, 0, buffer_context.vertex_buffer_capacity, 0,
                &buffer_context.data_vertex);

    // Zero out the entire buffer first
    memset(buffer_context.data_vertex, 0, buffer_context.vertex_buffer_capacity);
    // Then copy initial data
    memcpy(buffer_context.data_vertex, vertices.data(), initial_data_size);
    vkUnmapMemory(vulkan_context.logical_device, buffer_context.vertex_staging_buffer_memory);

    // Create device local buffer with full size
    buffer_create(vulkan_context, buffer_context.vertex_buffer_capacity,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer_context.vertex_buffer,
                  buffer_context.vertex_buffer_memory);

    // Copy entire buffer (including zeros for unused space)
    buffer_copy(vulkan_context, command_buffer_context, buffer_context.vertex_staging_buffer, buffer_context.vertex_buffer,
                buffer_context.vertex_buffer_capacity);


    std::cout << "CREATED TEXT VERTEX BUFFER SUCCESS (Size: " << buffer_context.vertex_buffer_capacity << " bytes for " << MAX_VERTICES <<
            " vertices)\n";
}

void text_vertex_buffer_update(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context,
                               Buffer_Context& buffer_context, Text_System& text_system)
{
    //TODO: so while not happening rn, we can use an offset to only copy the new data, instead of the whole buffer over
    //so instead of copy buffer we can use copy_buffer_region, which ill have to double check how it works

    // Only copy we are currently using vertices data
    VkDeviceSize current_vertex_data_size = sizeof(text_verticies[0]) * text_system.dynamic_vertices.size();

    //map and copy data
    //NOTE: YOU TECHNICALLY DONT NEED TO MAP AND UNMAP THE DATA, YOU CAN JUST COPY THE DATA OVER

    //vkMapMemory(vulkan_context.logical_device, command_buffer_context.vertex_staging_buffer_memory, 0, vertex_buffer_capacity, 0,
    //           &data_vertex);
    memcpy(buffer_context.data_vertex, text_system.dynamic_vertices.data(), current_vertex_data_size);
    //vkUnmapMemory(vulkan_context.logical_device, command_buffer_context.vertex_staging_buffer_memory);

    //transfer from storage buffer/cpu buffer to gpu buffer
    buffer_copy(vulkan_context, command_buffer_context, buffer_context.vertex_staging_buffer, buffer_context.vertex_buffer,
                current_vertex_data_size);
    //last param in copy_buffer used to be vertex_buffer_capacity and index_buffer_capacity respectively


    VkDeviceSize current_index_data_size = sizeof(indices[0]) * text_system.dynamic_indices.size();

    //vkMapMemory(vulkan_context.logical_device, command_buffer_context.index_staging_buffer_memory, 0, index_buffer_capacity, 0, &data_index);
    memcpy(buffer_context.data_index, text_system.dynamic_indices.data(), current_index_data_size);
    //vkUnmapMemory(vulkan_context.logical_device, command_buffer_context.index_staging_buffer_memory);

    buffer_copy(vulkan_context, command_buffer_context, buffer_context.index_staging_buffer, buffer_context.index_buffer,
                current_index_data_size);

}

void text_system_init()
{
}

bool load_font(Text_System& text_system, const char* filepath)
{
    unsigned char* ttf_buffer;

    // Load font file into memory
    //FOR TESTING: "c:/windows/fonts/arialbd.ttf"
    std::FILE* font_file = fopen(filepath, "rb");
    if (!font_file){ return false;}
    fseek(font_file, 0, SEEK_END);
    size_t size = ftell(font_file);
    rewind(font_file);

    ttf_buffer = (unsigned char*)malloc(size);
    fread(ttf_buffer, 1, size, font_file);
    fclose(font_file);

    // Initialize stb_truetype
    if (!stbtt_InitFont(&text_system.font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0)))
    {
        return false;
    };

    //get size and then generate bitmap
    float scale = stbtt_ScaleForPixelHeight(&text_system.font, 48.0f); // 48px font size

    for (int c = 32; c < 128; c++) {
        int width, height, xoff, yoff;

        // Rasterize this glyph
        unsigned char* bitmap = stbtt_GetCodepointBitmap(
            &text_system.font, 0, scale, c,
            &width, &height, &xoff, &yoff
        );

        // Advance metrics
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&text_system.font, c, &advance, &lsb);

        text_system.glyphs[c - 32].width   = width;
        text_system.glyphs[c - 32].height  = height;
        text_system.glyphs[c - 32].xoff    = xoff;
        text_system.glyphs[c - 32].yoff    = yoff;
        text_system.glyphs[c - 32].advance = advance * scale;
        text_system.glyphs[c - 32].bitmap  = bitmap;

        printf("Generated glyph '%c' (%dx%d)\n", c, width, height);
    }


    /*
    // Example: save one glyph bitmap for debugging
    // stored in cmake-build-debug
    FILE* out = fopen("glyph_A.pgm", "wb");
    fprintf(out, "P5\n%d %d\n255\n", text_system.glyphs['A' - 32].width, text_system.glyphs['A' - 32].height);
    fwrite(text_system.glyphs['A' - 32].bitmap, 1,
           text_system.glyphs['A' - 32].width * text_system.glyphs['A' - 32].height,
           out);
    fclose(out);
    */

    /* we dont do this cause, we need the info for the textures
    // Free all glyph bitmaps when done
    for (int i = 0; i < 96; i++) {
        stbtt_FreeBitmap(text_system.glyphs[i].bitmap, NULL);
    }*/

    free(ttf_buffer);


    return true;
}

bool load_font_into_atlas(Text_System& text_system, const char* filepath)
{
    return true;
}

void text_update(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context,
    Buffer_Context& buffer_context, Text_System& text_system)
{
    text_system.dynamic_indices.clear();
    text_system.dynamic_vertices.clear();

    do_text(text_system, "HIHIHI");

    text_vertex_buffer_update(vulkan_context, command_buffer_context, buffer_context, text_system);

}
