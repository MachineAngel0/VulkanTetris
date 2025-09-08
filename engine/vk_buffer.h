//
// Created by Adams Humbert on 9/7/2025.
//

#ifndef VK_BUFFER_H
#define VK_BUFFER_H

#include "vk_device.h"
#include "vulkan/vulkan.h"

struct Buffer_Context
{
    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;

    VkBuffer vertex_staging_buffer;
    VkDeviceMemory vertex_staging_buffer_memory;

    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;

    VkBuffer index_staging_buffer;
    VkDeviceMemory index_staging_buffer_memory;

    void* data_vertex;
    VkDeviceSize vertex_buffer_capacity = 0;
    void* data_index;
    VkDeviceSize index_buffer_capacity = 0;

};


inline void buffer_create(Vulkan_Context& vulkan_context){};
inline void buffer_destroy_free(Vulkan_Context& vulkan_context, Buffer_Context& buffer_context)
{
    vkDestroyBuffer(vulkan_context.logical_device, buffer_context.index_buffer, nullptr);
    vkFreeMemory(vulkan_context.logical_device, buffer_context.index_buffer_memory, nullptr);

    vkDestroyBuffer(vulkan_context.logical_device, buffer_context.vertex_buffer, nullptr);
    vkFreeMemory(vulkan_context.logical_device, buffer_context.vertex_buffer_memory, nullptr);

    vkDestroyBuffer(vulkan_context.logical_device, buffer_context.index_staging_buffer, nullptr);
    vkFreeMemory(vulkan_context.logical_device, buffer_context.index_staging_buffer_memory, nullptr);

    vkDestroyBuffer(vulkan_context.logical_device, buffer_context.vertex_staging_buffer, nullptr);
    vkFreeMemory(vulkan_context.logical_device, buffer_context.vertex_staging_buffer_memory, nullptr);
};
inline void buffer_copy(Vulkan_Context& vulkan_context){};

#endif //VK_BUFFER_H
