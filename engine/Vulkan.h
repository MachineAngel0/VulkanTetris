#ifndef VULKANFROMSPECS_H
#define VULKANFROMSPECS_H

#include <iostream>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <fstream>
#include <string>

#include <glm/glm.hpp>

//TODO: move out when done
void key_callback(GLFWwindow* window);


struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
};


const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};


const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};





static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    /*
    * VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex
    VK_VERTEX_INPUT_RATE_INSTANCE: Move to the next data entry after each instance
     */
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    //position
    attributeDescriptions[0].binding = 0; //referencing which VkVertexInputBindingDescription binding we are using
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos); //offsetof is pretty interesting

    //color
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

//ubo using a descriptor set
/*
* Specify a descriptor set layout during pipeline creation
*Allocate a descriptor set from a descriptor pool
*Bind the descriptor set during rendering
 */
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};



const std::vector<const char *> instance_extensions = {
    //VK_KHR_SURFACE_EXTENSION_NAME // this does not work it will cause the instance to fail
    //"VK_KHR_win32_surface",
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char *> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME
    //VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME// I in fact do not use an amd gpu
};

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct GLFW_Window_Context
{
    GLFWwindow* window;
    const char* WINDOW_NAME = "MadnessEngine";
    const int WIDTH = 800;
    const int HEIGHT = 600;
    bool framebufferResized = false;

};

//TODO: Split things out where appropriate
struct Vulkan_Context
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice logical_device;

    //TODO: might want to move these elsewhere (maybe)
    VkQueue graphics_queue;
    VkQueue present_queue;
};


struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Swapchain_Context
{
    VkSwapchainKHR swapchain;
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VkExtent2D surface_extent;
    //also contains VKformat
    VkSurfaceFormatKHR surface_format;
    VkPresentModeKHR presentModes;
    std::vector<VkImage> swap_chain_images;
    std::vector<VkImageView> swap_chain_image_views;
};

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
};


struct Graphics_Context
{
    VkRenderPass render_pass;
    std::vector<VkFramebuffer> frame_buffers;
    VkPipeline graphics_pipeline;
    VkPipelineLayout pipeline_layout;
};

struct Command_Buffer_Context
{
    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;

    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;

    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffer;
};



struct Semaphore_Fences_Context
{
    std::vector<VkSemaphore> image_available_semaphore;
    std::vector<VkSemaphore> render_finished_semaphore;
    std::vector<VkFence> in_flight_fence;
    //idk if its right for these two to be here, might be better in the swapchain context
    uint32_t currentFrame = 0;
};


/*MAIN LOOP*/
void run();


void main_loop(Vulkan_Context& vulkan_context,
                 GLFW_Window_Context& window_info,
                 Swapchain_Context& swapchain_context,
                 Graphics_Context& graphics_context,
                 Command_Buffer_Context& command_buffer_context,
                 Semaphore_Fences_Context& semaphore_fences_context);


/**/
void init_vulkan(Vulkan_Context& vulkan_context, GLFW_Window_Context& window_info,
    Swapchain_Context& swapchain_context, Graphics_Context& graphics_context,
    Command_Buffer_Context& command_buffer_context, Semaphore_Fences_Context& semaphore_fences_context);
//ensure we have validation layer support
bool ensure_validation_layer_support();
std::vector<const char*> getRequiredExtensions();


void draw_frame(Vulkan_Context& vulkan_context, GLFW_Window_Context& window_context, Swapchain_Context& swapchain_context,
                Graphics_Context& graphics_context, Command_Buffer_Context& command_buffer_context, Semaphore_Fences_Context& semaphore_fences_info);
/*CLEANUP*/
void cleanup(Vulkan_Context& vulkan_context, GLFW_Window_Context& window_info,
                 Swapchain_Context& swapchain_context, Graphics_Context& graphics_context,
                 Command_Buffer_Context& command_buffer_context, Semaphore_Fences_Context& semaphore_fences_context);
/* GLFW WINDOW*/
void init_window(GLFW_Window_Context& context);
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

/* VULKAN INSTANCE*/
void create_instance(Vulkan_Context& vulkan_context);
/*DEBUG MESSAGES*/
void create_debug_messanger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger);

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);;
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger);
VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

/* SURFACE */
void create_surface(Vulkan_Context& vulkan_context, GLFW_Window_Context& window_context);

/* PHYSICAL DEVICE*/
void pick_physical_device(Vulkan_Context& vulkan_context);

bool is_device_suitable(VkSurfaceKHR surface, VkPhysicalDevice physical_device_to_query);
QueueFamilyIndices find_queue_families(VkSurfaceKHR surface, VkPhysicalDevice physical_device);
bool queue_family_indices_is_complete(QueueFamilyIndices queue_family_indices);


/* LOGICAL DEVICE */
void create_logical_device(Vulkan_Context& vulkan_context);
/* SWAPCHAIN */
void create_swapchain(Vulkan_Context& vulkan_context, Swapchain_Context& swapchain_context);
VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& present_modes_available);
/* SWAPCHAIN RECREATION */
void recreate_swapchain(Vulkan_Context& vulkan_context, GLFW_Window_Context& window_context, Swapchain_Context& swapchain_context, Graphics_Context& graphics_context);
void cleanup_swapchain(Vulkan_Context& vulkan_context, Swapchain_Context& swapchain_context, Graphics_Context& graphics_context);



/* IMAGE VIEWS */
void create_image_views(Vulkan_Context& vulkan_context, Swapchain_Context& swapchain_context);
/* GRAPHICS PIPELINE*/
void create_graphics_pipeline(VkDevice& device, Swapchain_Context& swapchain_context, Graphics_Context& graphics_context);
std::vector<char> read_file(const std::string& filename);
VkShaderModule create_shader_module(VkDevice& logical_device, const std::vector<char>& code);
/*RENDER PASS*/ //TODO: replace with dynamic rendering local read
void create_render_pass(Vulkan_Context& vulkan_context, Swapchain_Context& swapchain_context, Graphics_Context& graphics_context);
/*FRAMEBUFFER*/ //TODO: replace with dynamic rendering local read
void create_frame_buffers(Vulkan_Context& vulkan_context, Swapchain_Context& swapchain_context, Graphics_Context& graphics_context);
/*COMMAND POOL*/
void create_command_pool(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
/*VERTEX BUFFER*/
void create_vertex_buffer(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
void create_buffer(Vulkan_Context& vulkan_context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
void copy_buffer(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_index, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
uint32_t findMemoryType(Vulkan_Context& vulkan_context, uint32_t typeFilter, VkMemoryPropertyFlags properties);

/*INDEX BUFFER*/
void create_index_buffer(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);


/*COMMAND BUFFER*/
void create_command_buffer(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
/*RECORD BUFFER*/
void record_command_buffer(Swapchain_Context& swapchain_context, Command_Buffer_Context& command_buffer_context, Graphics_Context& graphics_context, uint32_t image_index, uint32_t current_frame);
/*SEMAPHORES AND FENCES*/
void create_sync_objects(Vulkan_Context& vulkan_context, Semaphore_Fences_Context& semaphore_fences_info);



// Global dynamic vertex storage
inline std::vector<Vertex> dynamic_vertices;
inline std::vector<uint16_t> dynamic_indices;
inline bool vertex_buffer_needs_update = false;
inline bool e_key_pressed = false;


static VkBuffer vertex_staging_buffer;
static VkDeviceMemory vertex_staging_buffer_memory;
static void* data_vertex;
static VkDeviceSize vertex_buffer_capacity = 0;

static VkBuffer index_staging_buffer;
static VkDeviceMemory index_staging_buffer_memory;
static void* data_index;
static VkDeviceSize index_buffer_capacity = 0;

const uint32_t max_object_count = 1000;
const uint32_t vertices_per_object = 4;
const uint32_t indices_per_object = 6;
const uint32_t MAX_VERTICES = max_object_count * vertices_per_object;
const uint32_t MAX_INDICES = max_object_count * indices_per_object;

void add_quad(glm::vec2 pos, glm::vec3 color);
void update_vertex_buffer_recreate(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
std::vector<Vertex> create_quad(glm::vec2 pos, glm::vec3 color);


void update_vertex_buffer_update(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
void create_vertex_buffer_new(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);
void create_index_buffer_new(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context);

void copy_buffer_region(Vulkan_Context& vulkan_context, Command_Buffer_Context& command_buffer_context,
                        VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
                        VkDeviceSize srcOffset, VkDeviceSize dstOffset);

#endif //VULKANFROMSPECS_H
