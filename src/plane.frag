#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 color;

uniform sampler2D planeTex;
uniform sampler2D shadowMap;

float addShadow(vec4 fragPosLightSpace)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    float bias = 0.005;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0f;

    return shadow;
}

void main(void)
{
    vec4 texColor = texture(planeTex, TexCoord);

    float shadow = addShadow(FragPosLightSpace);

    if (texColor.a > 0.9 && shadow > 0.0f)
        color = vec4(0,0,0,shadow * 0.1) + texColor;
    else if (texColor.a > 0.9 && shadow == 0.0f)
        color = vec4(vec3(0.85f), 1.0f);
    else if (texColor.a < 0.9 && shadow > 0.0f)
        color = vec4(0,0,0,shadow * 0.1);
    else
        discard;
}
