#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = position;
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform vec4 u_colour;
uniform sampler2D u_texture;

void main()
{
    vec4 texColour = texture(u_texture, v_TexCoord);
    colour = texColour;
};