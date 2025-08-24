@echo off
C:/VulkanSDK/1.4.321.1/Bin/glslc.exe shader.vert -o vert.spv
C:/VulkanSDK/1.4.321.1/Bin/glslc.exe shader.frag -o frag.spv
C:/VulkanSDK/1.4.321.1/Bin/glslc.exe UI.vert -o uivert.spv
C:/VulkanSDK/1.4.321.1/Bin/glslc.exe UI.frag -o uifrag.spv
pause