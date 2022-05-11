#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

// 计算并记录深度值
void main()
{
    // 记录顶点位置到光源位置的距离
    float lightDistance = length(FragPos.xyz - lightPos);
    // 通过除以 far_plane 将 lightDistance 映射到 [0;1] 范围
    lightDistance = lightDistance / far_plane;
    
    // 顶点位置到光源位置的距离在 [0;1] 范围的映射为深度值
    gl_FragDepth = lightDistance;
}