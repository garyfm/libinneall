#version 460 core

layout (location = 0) in vec3 a_pos;

layout (std140, binding = 0) uniform RenderView {
    mat4 u_view;
    mat4 u_projection;
    vec3 u_view_pos;
};

uniform mat4 u_model;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}
