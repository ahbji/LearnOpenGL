#version 330 core
out vec4 FragColor;

uniform sampler2D ourTexture;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;

void main()
{
    FragColor = texture(ourTexture, TexCoords);
} 