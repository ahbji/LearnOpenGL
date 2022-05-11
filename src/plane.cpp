#include <glad/glad.h>

#include <plane.h>

Plane::Plane()
{
    setupVertices();
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

void Plane::draw(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
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
}