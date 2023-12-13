#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor =  texture(ourTexture, oTexCoord);
}