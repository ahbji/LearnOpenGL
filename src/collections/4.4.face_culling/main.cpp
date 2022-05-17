#include <game_window.h>
#include <learnopengl/filesystem.h>
#include <cube.h>
#include <plane.h>
#include <vertical_face.h>
#include <learnOpengl/shader.h>
#include <texture.h>

#include <glad/glad.h>
#include <stb_image.h>

#include <map>

using namespace learnGL;

Cube * cube;
Plane * plane;
VerticalFace * vegetation;
VerticalFace * transparentWindow;

Shader *cubeShader, *planeShader, *vegetationShader, *transparentWindowShader;
Texture *cubeTexture, *planeTexture, *vegetationTexture, *windowTexture;

void loopFunc();

std::vector<glm::vec3> vegetationPositions
{
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3( 1.5f, 0.0f, 0.51f),
    glm::vec3( 0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3 (0.5f, 0.0f, -0.6f)
};

std::vector<glm::vec3> windowPositions
{
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3( 1.5f, 0.0f, 0.51f),
    glm::vec3( 0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3( 0.5f, 0.0f, -0.6f)
};

void initBlending();

void setupCullFace(bool enable);

int main()
{
    window = initWindow("LearnOpenGL");

    initBlending();

    // 全局开启面剔除
    setupCullFace(true);

    if (window == nullptr)
    {
        return -1;
    }

    cubeShader = new Shader("face_culling_vertex.glsl", "face_culling_frag.glsl");
    cube = new Cube();
    cubeTexture = new Texture(FileSystem::getPath("resources/textures/marble.jpg").c_str());
    cubeTexture->setupTextureUnit(cubeShader, "texture1", Texture::DIFFUSE);

    planeShader = new Shader("face_culling_vertex.glsl", "face_culling_frag.glsl");
    plane = new Plane();
    planeTexture = new Texture(FileSystem::getPath("resources/textures/metal.png").c_str());
    planeTexture->setupTextureUnit(planeShader, "texture1", Texture::DIFFUSE);

    vegetationShader = new Shader("face_culling_vertex.glsl", "face_culling_frag.glsl");
    vegetation = new VerticalFace();
    vegetationTexture = new Texture(FileSystem::getPath("resources/textures/grass.png").c_str(), true, false);
    vegetationTexture->setupTextureUnit(vegetationShader, "texture1", Texture::DIFFUSE);

    transparentWindowShader = new Shader("face_culling_vertex.glsl", "face_culling_frag.glsl");
    transparentWindow = new VerticalFace();
    windowTexture = new Texture(FileSystem::getPath("resources/textures/window.png").c_str(), true, false);
    windowTexture->setupTextureUnit(transparentWindowShader, "texture1", Texture::DIFFUSE);

    mainLoop(loopFunc);
    
    closeWindow();
}

void initBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void setupCullFace(bool enable)
{
    if (enable)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
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
    glm::vec3 offset = glm::vec3(0.01f);

    // 临时关闭面剔除
    setupCullFace(false);
    glm::mat4 model = glm::mat4(1.0f);
    planeTexture->bind(GL_TEXTURE0);
    plane->draw(planeShader, model, view, projection);
    // 恢复面剔除
    setupCullFace(true);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cubeTexture->bind(GL_TEXTURE0);
    cube->draw(cubeShader, fixDepthFighting(model, offset), view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->draw(cubeShader, fixDepthFighting(model, offset), view, projection);

    // 临时关闭面剔除
    setupCullFace(false);
    for (unsigned int i = 0; i < vegetationPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetationPositions[i]);
        vegetationTexture->bind(GL_TEXTURE0);
        vegetation->draw(vegetationShader, fixDepthFighting(model, offset), view, projection);
    }

    // sort the transparent windows before rendering
    // ---------------------------------------------
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < windowPositions.size(); i++)
    {
        float distance = glm::length(camera.Position - windowPositions[i]);
        sorted[distance] = windowPositions[i];
    }

    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        windowTexture->bind(GL_TEXTURE0);
        transparentWindow->draw(transparentWindowShader, fixDepthFighting(model, offset), view, projection);
    }
    // 恢复面剔除
    setupCullFace(true);
}