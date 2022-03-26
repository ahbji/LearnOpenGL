```glsl
#version 330 core
out vec4 FragColor;

// 材质属性：
// 因为 ambient 在绝大多数情况等于 diffuse ，所以不需要
// diffuse 指定对应的纹理采样器
// 镜面反射和光泽度不变。
struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

// 环境光不变
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    // 使用 material.diffuse 采样 TexCoords 位置的纹理颜色
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // 使用 material.diffuse 采样 TexCoords 位置的纹理颜色
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
```