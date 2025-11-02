#version 460 core

in vec3 frag_pos;
out vec4 frag_color;

uniform vec3 set_color;

void main()
{
    vec3 color = abs(frag_pos);
    frag_color = vec4(color, 1.0f);
} 
