#version 330 core
struct PointLight
{
    vec3 position;
    vec3 color;

    float ambient;
    float diffuse;
    float specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight
{
    vec3 direction;
    vec3 color;

    float ambient;
    float diffuse;
    float specular;
};

vec3 addPointLight(PointLight pl, vec3 norm, vec3 frag, vec3 view);
vec3 addDirLight(DirLight dl, vec3 norm, vec3 frag, vec3 view);
float addShadow(vec4 FragPosLightSpace);

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
in vec4 FragPosLightSpace;
in mat3 TBN;
in vec3 T,B,N;

out vec4 color;

#define NPL 4
uniform PointLight pointLights[NPL];
uniform DirLight dirLight;

uniform vec3 viewPosition;

uniform vec3 ambientMaterial;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform float shininess;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view = normalize(viewPosition - FragPosition);
    vec3 result = vec3(0.0f);

//    norm = texture(normalMap, TexCoord).rgb;
//    norm = normalize(norm * 2.0 - 1.0);
//    norm = normalize(TBN * norm);
//    color = vec4(norm, 1.0f);
//    return;

    // Directional light
    result += addDirLight(dirLight, norm, FragPosition, view);

    // Point lights
    for (int i = 0; i < NPL; i++)
        result += addPointLight(pointLights[i], norm, FragPosition, view);

//    float shadow = addShadow(FragPosLightSpace);
//    result *= (1.0f - shadow);

    color = vec4(result, 1.0f);
}

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

//    float pcfDepth = texture(shadowMap, projCoords.xy).r;
//    shadow = currentDepth - bias > pcfDepth  ? 1.0 : 0.0;

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

vec3 addPointLight(PointLight pl, vec3 norm, vec3 frag, vec3 view)
{
    float dist = length(pl.position - frag);
    float atten = 1.0f / (pl.constant + dist*pl.linear + pow(dist, 2)*pl.quadratic);
    // Normalization is always a good practice
    vec3 lightDir = normalize(pl.position - frag);
    vec3 viewDir = normalize(view - frag);
    vec3 reflectDir = normalize(reflect(-lightDir, norm));

    vec3 ambient = pl.color * vec3(texture(diffuseTexture, TexCoord)) * pl.ambient * atten;
    vec3 diffuse = pl.color * vec3(texture(diffuseTexture, TexCoord)) * pl.diffuse * max(dot(norm, lightDir), 0.0) * atten;
    vec3 specular = pl.color * vec3(texture(specularTexture, TexCoord)) * pl.diffuse *
            pow(max(dot(reflectDir, viewDir), 0), shininess) * atten;

    return ambient + diffuse + specular;
}

vec3 addDirLight(DirLight dl, vec3 norm, vec3 frag, vec3 view)
{
    vec3 lightDir = normalize(-dl.direction);
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(view - frag);

    vec3 ambient = dl.color * vec3(texture(diffuseTexture, TexCoord)) * dl.ambient;
    vec3 diffuse = dl.color * vec3(texture(diffuseTexture, TexCoord)) * dl.diffuse * max(dot(norm, lightDir), 0.0);
    vec3 specular = dl.color * vec3(texture(specularTexture, TexCoord)) * dl.specular * pow(max(dot(reflectDir, viewDir), 0), shininess);

    float shadow = addShadow(FragPosLightSpace);

    return ambient + diffuse + specular;
//    return ambient + (1.0f - shadow) * (diffuse + specular);
}
