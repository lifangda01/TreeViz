#version 330 core

in vec3 TexCoord;

out vec4 color;

uniform samplerCube skybox;

void main(void)
{
     color = texture(skybox, TexCoord);
}

