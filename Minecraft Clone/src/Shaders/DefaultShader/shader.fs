#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;
in vec3 oColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor =  texture(ourTexture, oTexCoord) * vec4(oColor, 1);
}