#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 col;

uniform mat4 projection;
uniform mat4 view;

out vec3 Normal;
out vec3 Position;
out vec3 Color;

void main() 
{
    gl_Position = projection * view * vec4(pos, 1.0);

    Normal = norm;
    Position = pos;
    Color = col;
}