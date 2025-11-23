#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 uv;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vert_pos, 1.0);
    uv = vert_uv;
}
