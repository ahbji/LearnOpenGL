#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;   // 漫反射率
    sampler2D specular;  // 镜面反射率
    float shininess; // 光泽度
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

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform Material material;

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

uniform int sceneID;

uniform sampler2D shadowMap;

#define ID_MATERIAL_CUBE 0
#define ID_PLANE 1
#define ID_LIGHT_SRC_CUBE 2

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

vec4 gammaCorrection(vec4 color)
{
    float gamma = 2.2;
    color.rgb = pow(color.rgb, vec3(1.0/gamma));
    return color;
}

// 计算阴影元素（适用于透视投影和正交投影）
float ShadowCalculation(vec4 fragPosLightSpace, PointLight light)
{
    // 执行透视除法，将光空间坐标系中的 [-w, w] 范围转换到 [-1, 1] 范围，
    // 此操作只在应用透视投影矩阵时有意义。
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 将 NDC 坐标( [-1, 1] 范围)变换为 0 到 1 的范围
    // 为了和深度贴图的深度相比较， z 分量需要变换到 [0,1] ；
    // 为了作为从深度贴图中采样的坐标， x y 分量也需要变换到 [0,1] 。
    // 所以整个projCoords向量都需要变换到 [0,1] 范围。
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // 从阴影贴图中采样得到最近的深度值
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    // 得到当前片段的深度，等于来自光的透视视角的片段的深度。
    float currentDepth = projCoords.z;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // 使用阴影偏移解决阴影失真(Shadow Acne)
    // 1. 确定偏移值
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); // 偏移量的最大值0.05，和一个最小值0.005

    // 检查currentDepth是否高于 closestDepth ，如果是，那么片段就在阴影中
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF （percentage-closer filtering）处理
    // 简单的从纹理像素四周对深度贴图采样，然后把结果平均起来
    float shadow = 0.0;
    // 1. textureSize返回一个给定采样器纹理的 0 级 mipmap 的vec2类型的宽和高。
    // 用1除以它返回一个单独纹理像素的大小，我们用以对纹理坐标进行偏移，确保每个新样本，来自不同的深度值。
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    // 2. 在(x,y)上进行 9 此采样得到 9 个值，分布在 (x,y) 周边的 9 个点上。
    // 通过增加采样次数可以获得更淡的阴影效果。
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0; // 使用偏移值修正当前深度值
        }    
    }
    // 3. 最终通过样本的总数目将结果平均化
    shadow /= 9.0;

    // 解决采样过多：当光线的截锥体的 far_plane 区域外，保持阴影为 0.0 。  
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos); // 光线向量

    // 漫反射因子 
    float diff = max(dot(norm, lightDir), 0.0);
            
    // Blinn-Phong着色模型计算镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir); // 半向量
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess * 2); // 镜面反射因子，范围：0-1
    
    // 衰减率
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 阴影元素
    float shadow = ShadowCalculation(FragPosLightSpace, light); // 阴影外为 0.0，阴影内为 1.0

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    switch(sceneID)
    {
        case ID_MATERIAL_CUBE:
        {
            ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;     // 环境光分量
            diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; // 漫反射分量
            specular = light.specular * spec * texture(material.specular, TexCoords).rgb; // 镜面反射分量
            break;
        }
        case ID_PLANE:
        {
            vec3 color = texture(material.diffuse, TexCoords).rgb;
            ambient = light.ambient * color; // 环境光分量
            diffuse = light.diffuse * diff * color; // 漫反射分量
            specular = light.specular * spec; // 镜面反射分量
            break;
        }
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void scene()
{
    vec3 norm = normalize(Normal); // 法线
    vec3 viewDir = normalize(viewPos - FragPos); // 视线向量

    vec3 result = vec3(0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    vec4 fragColor = vec4(result, 1.0);
    // 着色前进行 gamma 矫正
    FragColor = gammaCorrection(fragColor);
}

void lightSrc()
{
    FragColor = vec4(1.0); // set alle 4 vector values to 1.0
}

void main()
{
    switch(sceneID)
    {
        case ID_LIGHT_SRC_CUBE:
            lightSrc();
            break;
        default:
            scene();
            break;
    }
}