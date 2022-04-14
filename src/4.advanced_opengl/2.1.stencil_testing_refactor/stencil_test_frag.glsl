#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool halo;

vec4 drawHalo();

void main()
{
	FragColor = halo ? drawHalo() : texture(texture1, TexCoord);
}

vec4 drawHalo()
{
	return vec4(0.04, 0.28, 0.26, 1.0);
}