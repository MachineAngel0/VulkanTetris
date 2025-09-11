cmake_minimum_required(VERSION 3.31)
project(VulkanTetris)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_UNITY_BUILD TRUE) # UNITY BUILD

find_package(Vulkan	REQUIRED)


add_subdirectory(lib/glfw-3.4)
add_subdirectory(lib/glm)

include_directories(VulkanTetris ./engine/)
include_directories(VulkanTetris ./lib/)
include_directories(VulkanTetris ./assets/)


add_executable(VulkanTetris main.cpp
        engine/Renderer.h
        engine/Renderer.cpp
        engine/Tetris.cpp
        engine/Tetris.h
        engine/Mesh.cpp
        engine/Mesh.h
        engine/clock.cpp
        engine/clock.h
        engine/UI.cpp
        engine/UI.h
        engine/texture.cpp
        engine/texture.h
        engine/vk_command_buffer.h
        engine/vk_command_buffer.cpp
        engine/vk_command_buffer.h
        engine/vk_device.cpp
        engine/vk_device.h
        engine/vk_buffer.cpp
        engine/vk_buffer.h
        engine/vk_vertex.cpp
        engine/vk_vertex.h
        engine/text.cpp
        engine/text.h
        lib/stb_impl.cpp
        engine/vk_descriptor.cpp
        engine/vk_descriptor.h
        engine/vk_renderpass.cpp
        engine/vk_renderpass.h
)




target_link_libraries(${PROJECT_NAME} PRIVATE
        Vulkan::Vulkan
        glfw
        glm
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Lib/freetype.lib
)

target_compile_definitions(VulkanTetris PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
        $<$<CONFIG:Release>:RELEASE_BUILD>

)

#Here because ill get .dll missing errors
# Force static linking approach for MinGW
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

# Also try to force GLFW to link statically if possible
set(GLFW_USE_STATIC_LIBS ON)

# Add static linking flags
target_link_options(VulkanTetris PRIVATE
        -static-libgcc
        -static-libstdc++
        -static
)