#include <game_window.h>
#include <learnopengl/filesystem.h>
#include <cube.h>
#include <plane.h>
#include <learnOpengl/shader.h>
#include <texture.h>

using namespace learnGL;

Cube *cube;
Plane *plane;
Shader *cubeShader, *planeShader;
Texture *cubeTexture, *planeTexture;

void loopFunc();


int main()
{
    window = initWindow("LearnOpenGL");

    // 禁用更新深度缓冲
    // glDepthMask(GL_FALSE);

    // 模拟禁用深度测试：深度测试将会永远通过，此时事实上后渲染的 fragment 的深度值永远比前渲染的深度值大，导致后绘制的 fragment 会遮挡先绘制的 frag
    // glDepthFunc(GL_ALWAYS);

    // Opengl 默认深度测试函数
    // glDepthFunc(GL_LESS);
    // glDepthFunc(GL_LEQUAL);

    // 模拟 glDepthMask(GL_FALSE);
    // glDepthFunc(GL_GREATER);
    // glDepthFunc(GL_GEQUAL);

    // 在片段深度值不等于缓冲区的深度值时通过测试
    // glDepthFunc(GL_NOTEQUAL);

    // 在片段深度值等于缓冲区的深度值时通过测试
    // glDepthFunc(GL_EQUAL);

    if (window == nullptr)
    {
        return -1;
    }

    cubeShader = new Shader("depth_test_vertex.glsl", "depth_test_frag.glsl");
    cube = new Cube();
    cubeTexture = new Texture(FileSystem::getPath("resources/textures/marble.jpg").c_str());
    cubeTexture->setupTextureUnit(cubeShader, "texture1", Texture::DIFFUSE);

    planeShader = new Shader("depth_test_vertex.glsl", "depth_test_frag.glsl");
    plane = new Plane();
    planeTexture = new Texture(FileSystem::getPath("resources/textures/metal.png").c_str());
    planeTexture->setupTextureUnit(planeShader, "texture1", Texture::DIFFUSE);

    mainLoop(loopFunc);
    
    closeWindow();
}

// 防止深度冲突
glm::mat4 fixDepthFighting(glm::mat4 model, glm::vec3 offset)
{
    return glm::translate(model, offset);
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
    cubeTexture->bind(GL_TEXTURE0);
    cube->draw(cubeShader, fixDepthFighting(model, glm::vec3(0.0001f)), view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->draw(cubeShader, fixDepthFighting(model, glm::vec3(0.0001f)), view, projection);
    
    model = glm::mat4(1.0f);
    planeTexture->bind(GL_TEXTURE0);
    plane->draw(planeShader, model, view, projection);
}