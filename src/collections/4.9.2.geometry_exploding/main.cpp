#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader.h>

#include <game_window.h>
#include <cube.h>

Shader* shader;
Model* nanosuit;

void loopFunc();

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    shader = new Shader("exploding_vertex.glsl", "exploding_frag.glsl", "exploding_geo.glsl");
    nanosuit = new Model(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
    
    mainLoop(loopFunc);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));

    
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // add time component to geometry shader in the form of a uniform
    shader->setFloat("time", static_cast<float>(glfwGetTime()));

    nanosuit->Draw(*shader);
}