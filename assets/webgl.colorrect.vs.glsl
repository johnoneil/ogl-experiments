#version 300 es
precision mediump float;

layout (location = 0) in vec3 vertex;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}