//
// Created by Adams Humbert on 9/6/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "vk_renderpass.h"
#include <stb_image.h>



/*TEXTURE IMAGE*/
void create_texture_image(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
void create_image(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
void transition_image_layout(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
void copyBufferToImage(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);


/*Texture Image Views*/
void create_texture_image_views(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
VkImageView create_image_view(Vulkan_Context& vulkan_context, VkImage image, VkFormat format);

/*Texture Sampler*/
void create_texture_sampler(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);



#endif //TEXTURE_H

