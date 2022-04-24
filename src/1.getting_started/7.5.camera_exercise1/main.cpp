#include <game_window.h>
#include <cube.h>

#include <learnopengl/filesystem.h>

Cube * cube;
unsigned int texture0, texture1;
void loopFunc();

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    cube = new Cube("7.6.cubeVertex.glsl", "7.6.cubeFrag.glsl");
    cube->setupVertices();
    texture0 = cube->loadMipMap(FileSystem::getPath("resources/textures/container.jpg").c_str(), "texture0", 0);
    texture1 = cube->loadMipMap(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), "texture1", 1);

    mainLoop(loopFunc);
}

void loopFunc()
{
    cube->bindTexture(GL_TEXTURE0, texture0);
    cube->bindTexture(GL_TEXTURE1, texture1);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        cube->draw(model, view, projection);
    }
}