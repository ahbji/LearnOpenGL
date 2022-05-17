#include <game_window.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <cube.h>
#include <plane.h>
#include <vertical_face.h>
#include <framebuffer.h>
#include <texture.h>
#include <map>

using namespace learnGL;

Cube *cube;
Plane *plane;
FrameBuffer *frameBuffer;
VerticalFace *vegetation, *transparentWindow;
Texture *cubeTexture, *planeTexture, *vegetationTexture, *windowTexture;
Shader *sceneShader, *framebufferShader;

void loopFunc();

void drawScenes();

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
void initBlend();

int main()
{
    window = initWindow("LearnOpenGL");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (window == nullptr)
    {
        return -1;
    }

    sceneShader = new Shader("scenes_vertex.glsl", "scenes_frag.glsl");

    cube = new Cube();
    cubeTexture = new Texture(FileSystem::getPath("resources/textures/container.jpg").c_str());
    cubeTexture->setupTextureUnit(sceneShader, "texture1", Texture::DIFFUSE);

    plane = new Plane();
    planeTexture = new Texture(FileSystem::getPath("resources/textures/metal.png").c_str());
    planeTexture->setupTextureUnit(sceneShader, "texture1", Texture::DIFFUSE);

    vegetation = new VerticalFace();
    vegetationTexture = new Texture(FileSystem::getPath("resources/textures/grass.png").c_str(), true, false);
    vegetationTexture->setupTextureUnit(sceneShader, "texture1", Texture::DIFFUSE);

    transparentWindow = new VerticalFace();
    windowTexture = new Texture(FileSystem::getPath("resources/textures/window.png").c_str(), true, false);
    windowTexture->setupTextureUnit(sceneShader, "texture1", Texture::DIFFUSE);

    framebufferShader = new Shader("framebuffer_vertex.glsl", "framebuffer_frag.glsl");
    frameBuffer = new FrameBuffer(SCR_WIDTH, SCR_HEIGHT);

    mainLoop(loopFunc);

    delete frameBuffer;
    delete cube;
    delete plane;
    delete vegetation;
    delete transparentWindow;
    frameBuffer = nullptr;
    cube = nullptr;
    plane = nullptr;
    vegetation = nullptr;
    transparentWindow = nullptr;
    
    closeWindow();
}

// 防止深度冲突
glm::mat4 fixDepthFighting(glm::mat4 model, glm::vec3 offset)
{
    return glm::translate(model, offset);
}

void drawScenes()
{
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::vec3 offset = glm::vec3(0.01f);

    glm::mat4 model = glm::mat4(1.0f);
    planeTexture->bind(GL_TEXTURE0);
    plane->draw(sceneShader, model, view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cubeTexture->bind(GL_TEXTURE0);
    cube->draw(sceneShader, fixDepthFighting(model, offset), view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->draw(sceneShader, fixDepthFighting(model, offset), view, projection);

    for (unsigned int i = 0; i < vegetationPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetationPositions[i]);
        vegetationTexture->bind(GL_TEXTURE0);
        vegetation->draw(sceneShader, fixDepthFighting(model, offset), view, projection);
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
        transparentWindow->draw(sceneShader, fixDepthFighting(model, offset), view, projection);
    }
}

void loopFunc()
{
    glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
    frameBuffer->draw(framebufferShader, drawScenes, bgColor);
}