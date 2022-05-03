#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal); // 法线
    vec3 viewDir = normalize(viewPos - FragPos); // 视线向量

    vec3 result = vec3(0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos); // 光线向量

    // 漫反射因子 
    float diff = max(dot(norm, lightDir), 0.0);
            
    // Blinn-Phong着色模型计算镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir); // 半向量
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess * 2); // 镜面反射因子，范围：0-1
    
    // 衰减率
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 color = texture(material.diffuse, TexCoords).rgb;
    vec3 ambient = light.ambient * color; // 环境光分量
    vec3 diffuse = light.diffuse * diff * color; // 漫反射分量
    vec3 specular = light.specular * spec; // 镜面反射分量

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}