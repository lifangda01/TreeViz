#version 330 core

flat in int Focus;  // Branch is current?

out vec4 color;

uniform vec3 color_in;

void main(void)
{
    float alpha = 0.7;
    if (Focus == 1) // Brnach is current
        color = vec4(0.7f, 0.2f, 0.4f, alpha);
    else if (Focus == 2)    // Showing diameter ranks
        color = vec4(color_in, alpha);
    else
        color = vec4(0.2f, 0.7f, 0.4f, alpha);
}
