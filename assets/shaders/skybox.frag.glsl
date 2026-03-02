#version 460 core

out vec4 frag_color;
in vec3 v_uv;

uniform samplerCube skybox;

void main()
{
    frag_color = texture(skybox, v_uv);
} 
