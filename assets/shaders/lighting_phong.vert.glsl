#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

layout (std140, binding = 0) uniform RenderView {
    mat4 u_view;
    mat4 u_projection;
    vec3 u_view_pos;
};

uniform mat4 u_model;
uniform mat3 u_normal_matrix;

out VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 uv;
} o_vertex;

void main()
{
    o_vertex.frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    o_vertex.normal = u_normal_matrix * a_normal;
    
    gl_Position = u_projection * u_view * vec4(o_vertex.frag_pos, 1.0);

    o_vertex.uv = a_uv;
}
