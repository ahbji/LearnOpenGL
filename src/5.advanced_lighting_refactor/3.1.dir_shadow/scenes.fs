#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

// 计算阴影元素（适用于透视投影和正交投影）
float ShadowCalculation(vec4 fragPosLightSpace)
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

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);

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

void main()
{ 
    vec3 lightDir = normalize(lightPos - fs_in.FragPos); 
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);

    // Blinn-Phong 光照模型
    // 环境光分量
    vec3 ambient = 0.3 * lightColor;
    // 漫反射分量
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // 镜面反射分量
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // 阴影元素
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace); // 阴影外为 0.0，阴影内为 1.0

    // 计算最终颜色
    // - 漫反射分量和镜面光分量参与阴影计算
    // - 由于阴影部分最终值为环境光分量
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}