#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;

uniform int sceneID;

#define ID_MATERIAL_CUBE 0
#define ID_PLANE 1
#define ID_LIGHT_SRC_CUBE 2

void scene()
{
    // 世界空间坐标系的顶点位置
    FragPos = vec3(model * vec4(aPos, 1.0));
    // 将世界空间坐标系的顶点位置转换到光空间坐标系
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

void lightSrcCube()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

void main()
{
    switch(sceneID)
    {
        case ID_LIGHT_SRC_CUBE:
            lightSrcCube();
            break;
        default:
            scene();
            break;
    }
}