#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;


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

    // 合并环境光和漫反射分量
    FragColor = vec4((ambient + diffuse) * objectColor, 1.0);
} 