

#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <vulkan/vulkan.h>
#include <vk_device.h>

struct Command_Buffer_Context
{
    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;

    VkBuffer vertex_staging_buffer;
    VkDeviceMemory vertex_staging_buffer_memory;

    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;

    VkBuffer index_staging_buffer;
    VkDeviceMemory index_staging_buffer_memory;

    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffer;


    //TODO: move out from commmand buffer struct
    VkImage texture_image;
    VkDeviceMemory texture_image_memory;
    VkImageView texture_image_view;
    VkSampler texture_sampler;

};

/*COMMAND POOL*/
void command_pool_allocate(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
void command_pool_free(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);


/*COMMAND BUFFER*/
void command_buffer_allocate(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context, uint32_t frames_in_flight);
void command_buffer_free(Vulkan_Context& vulkan_context);
VkCommandBuffer command_buffer_begin_single_use(Vulkan_Context& vulkan_context, VkCommandPool& command_pool);
void command_buffer_end_single_use(Vulkan_Context& vulkan_context, VkCommandPool& command_pool, VkCommandBuffer commandBuffer);



#endif //COMMAND_BUFFER_H
