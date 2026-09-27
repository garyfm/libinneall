#version 460 core

out vec4 frag_color;

uniform vec3 u_color;
uniform sampler2D u_map_albedo;
uniform bool u_use_texture = false;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 uv;
} i_frag;

void main()
{
    vec3 color;
    if(u_use_texture)
    {
        color = vec3(texture(u_map_albedo, i_frag.uv));
    }
    else
    {
        color = u_color;
    }
    frag_color = vec4(color, 1.0f);
} 
