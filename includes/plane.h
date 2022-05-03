#ifndef FLOOR_H
#define FLOOR_H

#include <learnopengl/shader.h>

class Plane
{
public:
    Shader* shader;

    enum MIPMAP_TYPE
    {
        DIFFUSE, // 对应 GL_TEXTURE0
        SPECULAR // 对应 GL_TEXTURE1
    };
    
    /**
     * @brief 使用两个 shader 构建 Plane 对象
     * 
     * @param vertexPath 顶点着色器文件路径
     * @param fragmentPath 片段着色器文件路径
     */
    Plane(const char* vertexPath, const char* fragmentPath);
    /**
     * @brief 使用三个 shader 构建 Plane 对象
     * 
     * @param vertexPath 顶点着色器文件路径
     * @param fragmentPath 片段着色器文件路径
     * @param geometryPath 几何着色器文件路径
     */
    Plane(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

    /**
     * @brief 初始化顶点数据，原点坐标在图片左下角，贴图分别在X轴和Y轴上重复两次
     */
    void setupVertices();

    /**
     * @brief 将2D素材加载为多级渐变纹理
     * 
     * @param texturePath 素材路径
     * @param sampler2DVariableName sampler2D 采样器名
     * @param textureUnitID 纹理单元 ID ，取值 0 -- 15 ，或 Plane::DIFFUSE + n ，n 为 0 -- 15
     * @return unsigned int 返回纹理对象指针
     */
    unsigned int loadMipMap(const char* texturePath, const std::string& sampler2DName ,unsigned int textureUnitID, bool gammaCorrection = false);

    /**
     * @brief 绑定纹理纹理单元
     * 
     * @param textureUnit GL_TEXTURE0 到 GL_TEXTURE15
     * @param texture 纹理对象指针
     */
    void bindTexture(GLenum textureUnit, unsigned int texture);

    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    ~Plane();
private:
    unsigned int VBO, VAO, EBO;
};
#endif
