#include <game_window.h>
#include <cube.h>

Cube * lightedCube;
Cube * lightSrcCube;

const float lightSrcSensitivity = SENSITIVITY * 4;

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

// 光源位置
glm::vec3 lightPos(-5.0f, 5.0f, 0.0f);

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    lightedCube = new Cube("lightting_vertex.glsl", "lightting_frag.glsl");
    lightedCube->setupVertices();

    lightSrcCube = new Cube("light_src_cube_vertex.glsl", "light_src_cube_frag.glsl");
    lightSrcCube->setupVertices();

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

    lightedCube->shader->use();
    lightedCube->shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightedCube->shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightedCube->shader->setVec3("lightPos", lightPos);
    lightedCube->shader->setVec3("viewPos", camera.Position);

    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        lightedCube->draw(model, view, projection);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightSrcCube->draw(model, view, projection);
}