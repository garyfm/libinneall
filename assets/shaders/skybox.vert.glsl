#version 460 core

layout (location = 0) in vec3 a_pos;

layout (std140, binding = 0) uniform RenderView {
    mat4 u_view;
    mat4 u_projection;
    vec3 u_view_pos;
};


out vec3 v_uv;

void main()
{
    v_uv = a_pos;
    mat4 view = mat4(mat3(u_view));
    gl_Position = u_projection * view * vec4(a_pos, 1.0);
}
