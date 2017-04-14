#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec4 FragPosLightSpace;

// model matrix is always identity
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main(void)
{
    gl_Position = projection * view * vec4(position, 1.0f);
    TexCoord = texCoord;
    FragPos = position;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
