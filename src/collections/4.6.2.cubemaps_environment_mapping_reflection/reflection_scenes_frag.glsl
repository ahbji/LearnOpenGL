#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{
	vec3 incidence = normalize(Position - viewPos);
    vec3 reflection = reflect(incidence, normalize(Normal));
    FragColor = vec4(texture(skybox, reflection).rgb, 1.0);
}