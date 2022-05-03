#include <glad/glad.h>
#include <stb_image.h>

#include <plane.h>

Plane::Plane(const char* vertexPath, const char* fragmentPath)
{
    shader = new Shader(vertexPath, fragmentPath);
}

Plane::Plane(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    shader = new Shader(vertexPath, fragmentPath, geometryPath);
}

void Plane::setupVertices()
{
    // 定义顶点数据
    // ------------------------------------------------------------------
    float vertices[] = {
        // 标准化设备位置坐标   // 法线向量        // texture 坐标
        //                                     设置为 2 ，结合 GL_REPEAT 纹理包围模式，贴图分别在X轴和Y轴上重复两次
        -5.0f, -0.5f,  -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 2.0f, // top-left
        5.0f,  -0.5f,  5.0f,  0.0f, 1.0f, 0.0f, 2.0f, 2.0f, // top-right
        5.0f,  -0.5f,  -5.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f  // bottom-right
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 2, 1, // 第一个三角形
        0, 3, 2  // 第二个三角形
    };
    // 创建 VAO 顶点数组对象
    glGenVertexArrays(1, &VAO);
    // 创建 VBO 顶点缓冲对象
    glGenBuffers(1, &VBO);
    // 创建 EBO 索引缓冲对象
    glGenBuffers(1, &EBO);
    // 绑定 VAO ，将后续顶点属性调用关联到 VAO 上。
    glBindVertexArray(VAO);
    // 绑定 vbo 到 GL_ARRAY_BUFFER 显存
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将顶点数据通过 vbo 写入 GL_ARRAY_BUFFER 显存
    // GL_STATIC_DRAW ：GPU 中数据不会改变。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 绑定 EBO 到 GL_ELEMENT_ARRAY_BUFFER 显存
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 将顶点索引数据通过 EBO 写入 GL_ELEMENT_ARRAY_BUFFER 显存
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置 OpenGL 从顶点数据中获取顶点位置坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置 OpenGL 从顶点数据中获取法线向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 设置 OpenGL 从顶点数据中获取顶点纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // 解绑 VAO
    glBindVertexArray(0);
}

unsigned int Plane::loadMipMap(const char* texturePath, const std::string& sampler2DName ,unsigned int textureUnitID, bool gammaCorrection)
{
    unsigned int texture;
    // 创建纹理对象
    glGenTextures(1, &texture);
    
    int width, height, nrChannels;
    // 因为 texture 坐标的原点在图片左下角，所以这里不需要反转 Y 轴
    stbi_set_flip_vertically_on_load(false);
    // 读取素材，并获取其宽、高、通道数
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        // 根据通道数设置纹理格式
        GLenum internalFormat, dataFormat;
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
        // 绑定纹理对象到 2D 纹理目标
        glBindTexture(GL_TEXTURE_2D, texture);
        // 为 2D 纹理目标生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        // 为 2D 纹理目标生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 设置2D纹理在 X 轴的包围方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // 设置2D纹理在 Y 轴的包围方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // 设置缩小操作时使用的纹理过滤方式，GL_LINEAR_MIPMAP_LINEAR 在两个邻近的多级渐变纹理之间使用线性插值进行采样 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // 设置放大操作时使用的纹理过滤方式， GL_LINEAR 生成更加羽化的纹理
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 释放素材数据
        stbi_image_free(data);

        // 设置 OpenGL 采样器对应的纹理单元 ID
        shader->use();
        shader->setInt(sampler2DName, textureUnitID);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        stbi_image_free(data);
    }
    // 返回纹理对象指针
    return texture;
}

void Plane::bindTexture(GLenum textureUnit, unsigned int texture) {
    // 绑定纹理对象到对应的纹理单元
    // 激活纹理单元
    glActiveTexture(textureUnit);
    // 绑定纹理对象到 GL_TEXTURE_2D 目标
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Plane::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    // 激活 shader
    shader->use();
    // projection * view * model 变换
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // 绘制平面
    // 绑定 VAO
    glBindVertexArray(VAO);
    // 使用索引绘制图形
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    // 绘制完毕，解绑 VAO
    glBindVertexArray(0);
}

Plane::~Plane()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader->ID);
    delete shader;
    shader = nullptr;
}