# Blinn-Phong着色模型

Blinn-Phong 着色模型相对于 Phong 着色模型，改进主要在与引入半程向量计算镜面反射因子

另外，相对于 Phone 着色模型，材质光泽度应用前需要加倍。

```glsl
// Blinn-Phong着色模型计算镜面反射
vec3 halfwayDir = normalize(lightDir + viewDir); // 半向量
float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess * 2); // 镜面反射因子，范围：0-1
```

