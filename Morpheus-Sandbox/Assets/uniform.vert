#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    mat4 TransformMatrix;
    float BreakValue;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.ProjectionMatrix * ubo.ViewMatrix * ubo.TransformMatrix * vec4(inPosition, 0.0, ubo.BreakValue);
    fragColor = inColor;
}