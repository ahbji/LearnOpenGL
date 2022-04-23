#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader.h>

#include <game_window.h>
#include <cube.h>

Shader* shader;
Model* planet;
Model* rock;

// 无明显掉帧最大数量
unsigned int amount = 13000;
float radius = 50.0;
float offset = 2.5f;

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

    shader = new Shader("planet_vertex.glsl", "planet_frag.glsl");
    planet = new Model(FileSystem::getPath("resources/objects/planet/planet.obj"));
    rock = new Model(FileSystem::getPath("resources/objects/rock/rock.obj"));

    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed

    for (unsigned int i = 0; i < amount; i++)
    {
        //  add to list of matrices
        modelMatrices[i] = measureAsteroidsPos(i);
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

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);

    planet->Draw(*shader);

    // draw meteorites
    for (unsigned int i = 0; i < amount; i++)
    {
        shader->setMat4("model", modelMatrices[i]);
        rock->Draw(*shader);
    }
}