#version 460 core

layout (location = 0) in vec3 vert_pos;

uniform mat4 transform;

out vec3 frag_pos;

void main()
{
    gl_Position = transform * vec4(vert_pos, 1.0);
    frag_pos = vert_pos;
}
