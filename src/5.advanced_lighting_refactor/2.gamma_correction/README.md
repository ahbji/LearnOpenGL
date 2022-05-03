# Gamma 校正

使用 gamma 校正，更新后颜色值可以更好显示，较暗的区域有显示更多细节。
总的来说，通过一个小修改得到了一个更好的图像质量。

## 全局 Gamma 校正

```glsl
glEnable(GL_FRAMEBUFFER_SRGB);
```

开启 GL_FRAMEBUFFER_SRGB 后，OpenGL 对所有片段都进行 Gamma 校正。

如果我们在最后输出之前就进行gamma校正，所有的后续操作都是在操作不正确的颜色值。例如，如果我们使用多个帧缓冲，我们会希望这样：
让两个帧缓冲之间传递的中间结果仍然保持线性空间颜色，只是给发送给监视器的最后的那个帧缓冲应用 gamma 校正。

## 在着色器中使用 Gamma 校正

不是所有片段都需要进行 Gamma 校正，这就需要在着色器代码中使用 Gamma 校正

```glsl
vec4 gammaCorrection(vec4 color);
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    vec4 fragColor = vec4(result, 1.0);

    // 着色前进行 gamma 校正
    FragColor = gammaCorrection(fragColor);
}

vec4 gammaCorrection(vec4 color)
{
    float gamma = 2.2;
    // 对 color 的 rgb 值进行 1/gamma 次幂操作 （color.rgb ^ {1/gamma}）
    color.rgb = pow(color.rgb, vec3(1.0/gamma));
    return color;
}
```

## 重校纹理

纹理素材的制作一般在 sRGB 色域下制作，如果在渲染纹理时进行 Gamma 校正，则会导致纹理亮度值过高，因为本质上对纹理进行了两次 Gamma 校正。

对纹理进行重校，使其回归线性色域空间可以解决这一问题。

OpenGL有两种对纹理进行重校：
1. 在着色器代码中对纹理进行纹理重校

```glsl
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    // 漫反射因子 
    float diff = max(dot(norm, lightDir), 0.0);     
    ...
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float gamma = 2.2;
    vec3 color = texture(material.diffuse, TexCoords).rgb;
    ...
    // 计算漫反射分量时对取样到的漫反射纹理进行 Gamma 重校
    vec3 diffuse = light.diffuse * diff * pow(color, vec3(gamma)); 
    ...
    diffuse *= attenuation;
    ...

    return (ambient + diffuse + specular);
}
```

为每个纹理素材都做此操作很麻烦，好在 OpenGL 为生成纹理的 glTexImage2D 函数提供了 GL_SRGB 和 GL_SRGB_ALPHA 两种选项，可以将纹理设置为 sRGB 色域空间，以便在生成纹理时自动将纹理转换为线性色域空间。

```c++
unsigned int loadMipMap(const char* texturePath, const std::string& sampler2DName ,unsigned int textureUnitID, bool gammaCorrection)
{
    ...
    int width, height, nrChannels;
    // 根据通道数设置纹理格式
    GLenum internalFormat, dataFormat;
    ...
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 1)
            internalFormat = dataFormat = GL_RED;
        else if (nrChannels == 3)
        {
            internalFormat = gammaCorrection ? 
                GL_SRGB // 重校纹理（将素材的色域从 sRGB 变回线性色域）
                : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            internalFormat = gammaCorrection ? 
                GL_SRGB_ALPHA // 重校纹理（将素材的色域从 sRGB 变回线性色域）
                : GL_RGBA;
            dataFormat = GL_RGBA;
        }
        ...
        // 为 2D 纹理目标生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        ...
    }
    ...
}
```

***并不是所有纹理素材都需要进行纹理重校，镜面反射贴图和法线贴图几乎都在线性空间，一般只需要对漫反射贴图进行纹理重校。***

## 光照衰减

LearnOpenGL 在基础光照中讨论的高级衰减方程在有 gamma 校正的场景下也是可用的。

如果场景中用不到上述方程，需要注意在应用 gamma 校正时使用 (1/distance) 计算衰减值：
```glsl
float attenuation = 1.0 / distance;
```