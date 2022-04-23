#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/Shader.h>

#include <game_window.h>
#include <cube.h>

unsigned int VBO;

Shader* planetShader;
Shader* asteroidShader;
Model* planet;
Model* rock;

// 无明显掉帧最大数量
unsigned int amount = 193000;
float radius = 75.0f;
float offset = 25.0f;

glm::mat4* modelMatrices;

void loopFunc();
glm::mat4 measureAsteroidsPos(unsigned int index);

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    camera.Position = glm::vec3(0.0f, 0.0f, 55.0f);

    planetShader = new Shader("planet_vertex.glsl", "planet_frag.glsl");
    asteroidShader = new Shader("asteroids_vertex.glsl", "asteroids_frag.glsl");
    planet = new Model(FileSystem::getPath("resources/objects/planet/planet.obj"));
    rock = new Model(FileSystem::getPath("resources/objects/rock/rock.obj"));

    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed

    for (unsigned int i = 0; i < amount; i++)
    {
        //  add to list of matrices
        modelMatrices[i] = measureAsteroidsPos(i);
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < rock->meshes.size(); i++)
    {
        unsigned int VAO = rock->meshes[i].VAO;
        GLsizei vec4Size = sizeof(glm::vec4);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    }
    mainLoop(loopFunc);
}

glm::mat4 measureAsteroidsPos(unsigned int index)
{
    glm::mat4 model = glm::mat4(1.0f);
    // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
    float angle = (float)index / (float)amount * 360.0f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // 2. 缩放：在 0.05 和 0.25f 之间缩放
    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));

    // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
    float rotAngle = (rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
    return model;
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();;

    asteroidShader->use();
    asteroidShader->setMat4("projection", projection);
    asteroidShader->setMat4("view", view);

    planetShader->use();
    planetShader->setMat4("projection", projection);
    planetShader->setMat4("view", view);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    planetShader->setMat4("model", model);

    planet->Draw(*planetShader);

    asteroidShader->use();
    asteroidShader->setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock->textures_loaded[0].id);
    for(unsigned int i = 0; i < rock->meshes.size(); i++)
    {
        glBindVertexArray(rock->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}