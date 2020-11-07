#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
} ubo;

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = ubo.ProjectionMatrix * ubo.ViewMatrix * vec4(inPosition.xyz, 1.0);
    fragColor = inColor;
}