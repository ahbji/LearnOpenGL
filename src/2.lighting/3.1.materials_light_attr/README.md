在片段着色器中，创建一个结构体(Struct)，分别储存物体材质属性和光源属性：
- 物体本身的颜色 objectColor 分解为材质属性
- 光源颜色 lightColor 则分解为光源属性

这样，就可以将冯氏光照模型进一步分解为材质和光源两部分，从而更好地控制光照效果。

```glsl
#version 330 core
out vec4 FragColor;

// 定义材质属性
struct Material
{
    vec3 ambient; // 物体所呈现的颜色
    vec3 diffuse; // 物体所呈现的颜色
    vec3 specular; // 物体反射光的比例
    float shininess; // 光泽度：镜面高光的散射半径
};
uniform Material material;

// 定义光源属性
struct Light {
    vec3 position; // 光源位置

    vec3 ambient; // 环境光
    vec3 diffuse; // 漫反射光
    vec3 specular; // 镜面反射光
};
uniform Light light;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 viewPos;

void main()
{
    // 根据光源属性和材质属性计算各个光照分量
    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射光
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 镜面高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 
```