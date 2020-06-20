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
        
layout(location = 0) out vec4 FragColour;

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
    // Find material values from textures
    vec3 albedo = pow(texture(u_albedoMap, TexCoords).rgb, vec3(2.2)); 
    float metallic = texture(u_metallicMap, TexCoords).r;
    float roughness = texture(u_roughnessMap, TexCoords).r;
    float ao = texture(u_aoMap, TexCoords).r;

    vec3 N = normalize(Normal); // Surface normal - Normal Map not currently used
    vec3 V = normalize(u_cameraPosition - WorldPos); // View direction vector

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0f); // Initial Radiance which will be calculated using the render equation
    for (int i = 0; i < 1; ++i) { // Loop through each light in scene
        vec3 L = normalize(u_lightPositions[i] - WorldPos); // Light direction vector
        vec3 H = normalize(V + L); // Halfway vector between light and view

        float distance = length(u_lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance); // Calculate the light falloff using the inverse square law
        vec3 radiance = u_lightColours[i] * attenuation;

        // Cook torrance is used for the specular half of the calculation
        // Cook torrance = (D*F*G)/(4(w0.n)(wi.n))
        // Where:
        // D = a normal distribution function for determining the amound of surface
        //     aligned with the halfway vector
        //
        // F = the Fresnel equation used to describe surface reflection at a given angle
        //
        // G = a geometry function, that describes how a microfacet may shaddow another microfacet

        // Calculate D, G and F
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        // Calculate the Cook-Torrance specular value
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001); // Ensure divide by 0 does not occur

       
        vec3 kS = F; // Specular ratio

        vec3 kD = vec3(1.0) - kS; // Diffusion ratio
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);


        // Cook-Torrance reflectance
        // = kF * Lambert + kS * Cook-Torrance-Specular * N.L
        // Using Lambert diffusion
        // Lambert = albedo/Pi
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }

    vec3 ambient = vec3(0.03) * albedo * ao; //Create an initial ambient collour

    vec3 colour = ambient + Lo; // Add the radiance
    colour = colour / (colour + vec3(1.0)); // Gamma correction
    colour = pow(colour, vec3(1.0 / 2.2));

    FragColour = vec4(colour, 1.0f); // Output final colour
};