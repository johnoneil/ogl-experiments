#version 300 es
precision mediump float;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = aTexCoords;
}