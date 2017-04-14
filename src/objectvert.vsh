#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosition;
out vec4 FragPosLightSpace;
out mat3 TBN;
out vec3 T,B,N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = texCoord;
    // Light source is fixed in world space, thus translate normal from model to world space as well
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPosition = vec3(model * vec4(position, 1.0));
//    FragPosition = position;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPosition, 1.0);

    T = normalize(vec3(model * vec4(tangent,   0.0)));
    N = normalize(vec3(model * vec4(normal,    0.0)));
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    B = cross(T, N);
    TBN = mat3(T, B, N);
}
