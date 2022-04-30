#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;   // 环境光
    vec3 diffuse;   // 漫反射率
    vec3 specular;  // 镜面反射率
    float shininess; // 光泽度
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;  
in vec3 FragPos;  
  
uniform Material material;
uniform Light light;

uniform vec3 viewPos; 

void main()
{
    // 环境光
    vec3 ambient = light.ambient * material.ambient;
  	
    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // 漫反射因子，范围：0-1
    vec3 materialDiff = diff * material.diffuse; // 使用漫反射因子计算材质的漫反射分量
    vec3 diffuse = light.diffuse * (materialDiff); // 漫反射分量
            
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos); // 视线向量
    vec3 reflectDir = reflect(-lightDir, norm); // 反射方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 镜面反射因子，范围：0-1
    vec3 specular = light.specular * (spec * material.specular); // 镜面反射分量

    // 合并分量结果    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 