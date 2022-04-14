#include <game_window.h>
#include <learnopengl/filesystem.h>
#include <cube.h>
#include <plane.h>

Cube * cube;
Plane * plane;

unsigned int cubeTexture, planeTexture;

bool stencilTest = false;

void loopFunc();
void setupStencileTest();

int main()
{
    window = initWindow("LearnOpenGL");

    setupStencileTest();

    if (window == nullptr)
    {
        return -1;
    }

    cube = new Cube("stencil_test_vertex.glsl", "stencil_test_frag.glsl");
    cube->setupVertices();
    cubeTexture = cube->loadMipMap(FileSystem::getPath("resources/textures/marble.jpg").c_str(), "texture1", Cube::DIFFUSE);

    plane = new Plane("stencil_test_vertex.glsl", "stencil_test_frag.glsl");
    plane->setupVertices();
    planeTexture = plane->loadMipMap(FileSystem::getPath("resources/textures/metal.png").c_str(), "texture1", Plane::DIFFUSE);
    
    mainLoop(loopFunc);

    closeWindow();
}

void setupStencileTest()
{
    stencilTest = true;
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
}


glm::mat4 fixDepthFighting(glm::mat4 model, glm::vec3 offset)
{
    return glm::translate(model, offset);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // 确保在绘制不带边框的图形时，不更新深度缓冲区
    if (stencilTest)
    {
        glStencilMask(0x00);
    }

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::vec3 scale = glm::vec3(1.1f);
    glm::vec3 offset = glm::vec3(0.01f);

    glm::mat4 model = glm::mat4(1.0f);

    // 绘制地板
    plane->bindTexture(GL_TEXTURE0, planeTexture);
    plane->draw(model, view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -3.0f));

    // 绘制 cube
    cube->bindTexture(GL_TEXTURE0, cubeTexture);
    cube->draw(fixDepthFighting(model, offset), view, projection);

    // 绘制 halo cube
    cube->bindTexture(GL_TEXTURE0, cubeTexture);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->drawWithHalo(fixDepthFighting(model, offset), view, projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cube->drawWithHalo(fixDepthFighting(model, offset), view, projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, 1.0f));
    cube->drawWithHalo(fixDepthFighting(model, offset), view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->drawHalo(fixDepthFighting(model, offset), scale, view, projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cube->drawHalo(fixDepthFighting(model, offset), scale, view, projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, 1.0f));
    cube->drawHalo(fixDepthFighting(model, offset), scale, view, projection);
}