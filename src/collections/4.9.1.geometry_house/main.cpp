#include <game_window.h>
#include <glad/glad.h>
#include <learnopengl/shader.h>

#include <learnopengl/filesystem.h>

void loopFunc();

float points[] = {
    // aPos       // aColor
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // 右上
    0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // 右下
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};
unsigned int VBO, VAO;
Shader* shader;

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    shader = new Shader("point_vertex.glsl", "point_frag.glsl", "point_geo.glsl");

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    mainLoop(loopFunc);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 4);
}