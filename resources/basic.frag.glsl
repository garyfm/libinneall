#version 460 core

out vec4 frag_color;

uniform vec3 set_color;

void main()
{
    frag_color = vec4(set_color, 1.0f);
} 
