#version 460 core

layout (location = 0) in vec3 vert_pos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 frag_pos;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vert_pos, 1.0);
    frag_pos = vert_pos;
}
