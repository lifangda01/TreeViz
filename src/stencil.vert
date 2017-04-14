#version 330 core

layout (location = 0) in vec3 position;

flat out int Focus;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int focus;

void main(void)
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    Focus = focus;
}
