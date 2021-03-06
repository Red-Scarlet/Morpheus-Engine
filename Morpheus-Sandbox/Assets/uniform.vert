#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(binding = 0) uniform UniformBufferObject {
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    mat4 TransformMatrix;
} ubo;

void main() 
{
    gl_Position = ubo.ProjectionMatrix * ubo.ViewMatrix * ubo.TransformMatrix * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}