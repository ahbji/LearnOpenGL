#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos; // 片段的世界空间坐标
    vec2 TexCoords; // 纹理的世界空间u欧标
    vec3 TangentLightPos; // 光源的切线空间坐标
    vec3 TangentViewPos; // 观察者的切线空间坐标
    vec3 TangentFragPos; // 片段的切线空间坐标
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main()
{           
    // 从法线贴图范围[0,1]获取法线
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // 将法线向量转换为范围[-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // 此 normal 在切线空间

    // 处理光照
    // 视线在切线空间的向量
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    // 光线在切线空间的向量
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    
    // 从漫反射纹理中采样片段的漫反射率
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    // 计算漫反射因子
    float diff = max(dot(lightDir, normal), 0.0);
    // 计算镜面反射因子
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // 计算环境光
    vec3 ambient = 0.1 * color;
    // 计算漫反射率
    vec3 diffuse = diff * color;
    // 计算镜面反射率
    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}