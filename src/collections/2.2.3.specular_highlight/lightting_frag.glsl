#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 viewPos;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

const int SHININESS = 32; // 光泽度

void main()
{
    // 环境光
    float ambientStrength = 0.1; 
    vec3 ambient = ambientStrength * lightColor; // 环境因子 * 光颜色
  	
    // 漫反射
    vec3 norm = normalize(Normal); // 顶点法向量
    vec3 lightDir = normalize(lightPos - FragPos);  // 光线方向
    float diff = max(dot(norm, lightDir), 0.0); // 计算漫反射因子，范围：0-1
    vec3 diffuse = diff * lightColor; // 漫反射因子 * 光颜色

    // 镜面反射
    float specularStrength = 0.5; // 镜面反射强度
    vec3 viewDir = normalize(viewPos - FragPos); // 视线方向
    vec3 reflectDir = reflect(-lightDir, norm); // 反射方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS); // 镜面反射因子
    vec3 specular = specularStrength * spec * lightColor; // 镜面反射强度 * 镜面反射因子 * 光颜色（通过向量数乘计算）

    // 合并各个分量
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
} 