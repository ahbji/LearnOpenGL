#include <game_window.h>
#include <learnopengl/filesystem.h>
#include <cube.h>
#include <plane.h>
#include <learnOpengl/shader.h>

Cube * cube;
Plane * plane;
Shader *cubeShader, *planeShader;

void loopFunc();


int main()
{
    window = initWindow("LearnOpenGL");

    if (window == nullptr)
    {
        return -1;
    }

    cubeShader = new Shader("depth_test_vertex.glsl", "depth_test_frag.glsl");
    cube = new Cube();

    planeShader = new Shader("depth_test_vertex.glsl", "depth_test_frag.glsl");
    plane = new Plane();

    mainLoop(loopFunc);
    
    closeWindow();
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
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cube->draw(cubeShader, model, view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->draw(cubeShader, model, view, projection);
    
    model = glm::mat4(1.0f);
    plane->draw(planeShader, model, view, projection);
}