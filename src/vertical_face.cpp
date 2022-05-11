#include <glad/glad.h>

#include <vertical_face.h>

TransparentVerticalFace::TransparentVerticalFace()
{
    setupVertices();
}

void TransparentVerticalFace::setupVertices()
{
    // 定义标准化设备坐标数据
    // ------------------------------------------------------------------
    float vertices[] = {
        // 位置               // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, // top-left
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f, // bottom-left
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f, // bottom-right

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, // top-left
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f, // bottom-right
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f  // top-right
    };
    // 创建 vao 顶点数组对象
    glGenVertexArrays(1, &VAO);
    // 创建 vbo 顶点缓冲对象
    glGenBuffers(1, &VBO);

    // 绑定 vao ，将后续顶点属性调用关联到 vao 上。
    glBindVertexArray(VAO);

    // 绑定 vbo 到 GL_ARRAY_BUFFER 显存
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将顶点数据通过 vbo 写入 GL_ARRAY_BUFFER 显存
    // GL_STATIC_DRAW ：GPU 中数据不会改变。
    // GL_DYNAMIC_DRAW/GL_STREAM_DRAW ：GPU 中数据会改变，因此写入能高速访问的显存 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void TransparentVerticalFace::draw(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    // activate shader
    shader->use();
    shader->setMat4("projection", projection);
    // camera/view 变换
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // 绑定 vao ，绘制立面
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

TransparentVerticalFace::~TransparentVerticalFace()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}