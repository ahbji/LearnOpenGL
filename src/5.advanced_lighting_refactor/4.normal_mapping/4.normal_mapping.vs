#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos; // 片段的世界空间坐标
    vec2 TexCoords; // 纹理的世界空间坐标
    vec3 TangentLightPos; // 光源的切线空间坐标
    vec3 TangentViewPos; // 观察者的切线空间坐标
    vec3 TangentFragPos; // 片段的切线空间坐标
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;
    
    // 计算 TBN 矩阵的转置矩阵
    // ---------------------
    // vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    // vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    // vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    // mat3 TBN = transpose(mat3(T, B, N)); // 正交矩阵的逆矩阵和转置矩阵是一样的，用转置来代替逆矩阵计算量更小
    // ----------使用格拉姆-施密特正交化过程对TBN向量进行重正交化-----------
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    // 将世界坐标空间的向量转换到切线坐标空间
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}