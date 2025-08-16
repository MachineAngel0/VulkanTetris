#version 450

layout(binding = 0) in uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 incolor;

layout(location = 0) out vec3 fragColor;

void main() {
    inPosition.x /= 600;
    inPosition.y /= 600;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    fragColor = incolor;
}