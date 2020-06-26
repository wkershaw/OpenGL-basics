#shader vertex
#version 330 core
        
layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec2 lUv;
layout(location = 2) in vec3 lNormal;


uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;


void main()
{
    vec3 WorldPos = vec3(u_model * vec4(lPosition, 1.0));
    gl_Position = u_proj * u_view * vec4(WorldPos, 1.0);
};


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 colour;

void main(){
    colour = vec4(1,1,1,1);

};