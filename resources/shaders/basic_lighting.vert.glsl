#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_normal_matrix;

out vec3 v_frag_pos;
out vec3 v_normal;
out vec2 v_uv;

void main()
{
    v_frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    v_normal = u_normal_matrix * a_normal;
    
    gl_Position = u_projection * u_view * vec4(v_frag_pos, 1.0);

    v_uv = a_uv;
}
