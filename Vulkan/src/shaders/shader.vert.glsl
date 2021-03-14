#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform MVP {
    mat4 view;
    mat4 proj;
} mvp;

layout(push_constant) uniform Model {
    mat4 model;
} model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = mvp.proj * mvp.view * model.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}