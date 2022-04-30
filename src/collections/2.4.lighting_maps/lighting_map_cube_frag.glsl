#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse; // 采样漫反射贴图：环境光几乎等于漫反射率
    sampler2D specular; // 采样镜面光贴图
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords; // 纹理坐标
  
uniform Material material;
uniform Light light;

uniform vec3 viewPos; 

void main()
{
    // ambient                     // 从纹理中采样片段的漫反射率
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal); // 顶点法向量
    vec3 lightDir = normalize(light.position - FragPos); // 光线方向
    float diff = max(dot(norm, lightDir), 0.0); // 计算漫反射因子，范围：0-1
    // 漫反射因子 * 光颜色                  // 从漫反射贴图纹理中采样片段的漫反射率
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
            
    // specular
    vec3 viewDir = normalize(viewPos - FragPos); // 视线方向
    vec3 reflectDir = reflect(-lightDir, norm);  // 反射方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 镜面反射因子
    // 光的镜面反射 * 镜面反射因子 * 材质片段的镜面反射率（从镜面光贴图纹理中采样）
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}