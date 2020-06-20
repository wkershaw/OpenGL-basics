#shader vertex
#version 330 core
        
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec2 l_Uv;
layout(location = 2) in vec3 l_Normal;


uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

void main()
{

    TexCoords = l_Uv;
    WorldPos = vec3(u_model * vec4(l_Position, 1.0));
    Normal = mat3(u_model) * l_Normal;

    gl_Position = u_proj * u_view * vec4(WorldPos, 1.0);
};


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 colour;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 u_cameraPosition;
uniform vec3 u_lightPosition;

uniform sampler2D u_albedoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_metallicMap;
uniform sampler2D u_roughnessMap;
uniform sampler2D u_aoMap;

uniform vec3 u_lightPositions[1];
uniform vec3 u_lightColours[1];

const float PI = 3.14159265359;


vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(WorldPos);
    vec3 Q2 = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}


//Fresnel-Schlick approximation finds the ratio between reflection and refraction
//(Specular : Diffuse)
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


void main()
{
    vec3 albedo = pow(texture(u_albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic = texture(u_metallicMap, TexCoords).r;
    float roughness = texture(u_roughnessMap, TexCoords).r;
    float ao = texture(u_aoMap, TexCoords).r;


    vec3 N = normalize(Normal);
    vec3 V = normalize(u_cameraPosition - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0f);
    for (int i = 0; i < 4; ++i) {
        vec3 L = normalize(u_lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);

        float distance = length(u_lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_lightColours[i] * attenuation;


        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);


        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

       
        vec3 kS = F;

        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    colour = vec4(color, 1.0f);
};