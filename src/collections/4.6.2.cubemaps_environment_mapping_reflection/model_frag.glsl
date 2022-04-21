#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;
uniform samplerCube skybox;

void main()
{
    // Diffuse
    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);

    // Reflection
    vec3 incidence = normalize(Position - viewPos);
    vec3 reflection = reflect(incidence, normalize(Normal));
    float reflect_intensity = texture(texture_reflection1, TexCoords).r;
    vec4 reflect_color;
    if(reflect_intensity > 0.1) // Only sample reflections when above a certain treshold
        reflect_color = texture(skybox, reflection) * reflect_intensity;

    FragColor = diffuse_color + reflect_color;
}