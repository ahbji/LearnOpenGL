#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader.h>

#include <game_window.h>
#include <cube.h>

Shader* shader;
Shader* normalShader;
Model* nanosuit;

void loopFunc();

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    shader = new Shader("model_vertex.glsl", "model_frag.glsl");
    normalShader = new Shader("normal_visualization_vertex.glsl", "normal_visualization_frag.glsl", "normal_visualization_geo.glsl");
    stbi_set_flip_vertically_on_load(true);
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

    nanosuit->Draw(*shader);

    // then draw model with normal visualizing geometry shader
    normalShader->use();
    normalShader->setMat4("projection", projection);
    normalShader->setMat4("view", view);
    normalShader->setMat4("model", model);

    nanosuit->Draw(*normalShader);
}