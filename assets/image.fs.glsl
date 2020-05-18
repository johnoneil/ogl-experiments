#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform vec4 c;
uniform sampler2D Texture;

void main()
{    
    vec4 sampled = texture(Texture, TexCoord);
    color = c * sampled;
    //color = vec4(1,0,0,1);
}