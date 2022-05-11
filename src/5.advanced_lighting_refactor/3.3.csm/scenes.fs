#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2DArray shadowMap;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform float farPlane;

uniform mat4 view;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1

float ShadowCalculation(vec3 fragPosWorldSpace)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }
    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);

    // 执行透视除法，将光空间坐标系中的 [-w, w] 范围转换到 [-1, 1] 范围，
    // 此操作只在应用透视投影矩阵时有意义。
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 所以整个projCoords向量都需要变换到 [0,1] 范围。
    projCoords = projCoords * 0.5 + 0.5;

    // 获取从光视角观察到的当前片段的深度值
    float currentDepth = projCoords.z;
    // 解决采样过多：当光线的截锥体的 far_plane 区域外，保持阴影为 0.0 。  
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    vec3 normal = normalize(fs_in.Normal);
    // 使用阴影偏移解决阴影失真(Shadow Acne)
    // 1. 确定偏移值 (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    const float biasModifier = 0.5f;
    if (layer == cascadeCount)
    {   
        // 当前层是最后一层
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        // 当前层不是最后一层
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF （percentage-closer filtering）处理
    // 简单的从纹理像素四周对深度贴图采样，然后把结果平均起来
    float shadow = 0.0;
    // 1. textureSize返回一个给定采样器纹理的 0 级 mipmap 的vec2类型的宽和高。
    // 用1除以它返回一个单独纹理像素的大小，我们用以对纹理坐标进行偏移，确保每个新样本，来自不同的深度值。
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    // 2. 在(x,y)上进行 9 此采样得到 9 个值，分布在 (x,y) 周边的 9 个点上。
    // 通过增加采样次数可以获得更淡的阴影效果。
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    // 3. 最终通过样本的总数目将结果平均化
    shadow /= 9.0;
        
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
   // Blinn-Phong 光照模型
    // 环境光分量
    vec3 ambient = 0.3 * color;
    // 漫反射分量
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // 镜面反射分量
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // 阴影元素
    float shadow = ShadowCalculation(fs_in.FragPos);
    // 计算最终颜色
    // - 漫反射分量和镜面光分量参与阴影计算
    // - 由于阴影部分最终值为环境光分量
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}