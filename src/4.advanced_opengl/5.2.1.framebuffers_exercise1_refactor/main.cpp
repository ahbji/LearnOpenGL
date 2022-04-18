#include <game_window.h>
#include <learnopengl/filesystem.h>
#include <cube.h>
#include <plane.h>
#include <vertical_face.h>
#include <framebuffer.h>

#include <map>

Cube * cube;
Plane * plane;
FrameBuffer * frameBuffer;
TransparentVerticalFace * vegetation;
TransparentVerticalFace * transparentWindow;

unsigned int cubeTexture, planeTexture, vegetationTexture, windowTexture;
void loopFunc();

void drawMirror();
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

    cube = new Cube("scenes_vertex.glsl", "scenes_frag.glsl");
    cube->setupVertices();
    cubeTexture = cube->loadMipMap(FileSystem::getPath("resources/textures/container.jpg").c_str(), "texture1", Cube::DIFFUSE);

    plane = new Plane("scenes_vertex.glsl", "scenes_frag.glsl");
    plane->setupVertices();
    planeTexture = plane->loadMipMap(FileSystem::getPath("resources/textures/metal.png").c_str(), "texture1", Plane::DIFFUSE);

    vegetation = new TransparentVerticalFace("scenes_vertex.glsl", "scenes_frag.glsl");
    vegetation->setupVertices();
    vegetationTexture = vegetation->loadMipMap(FileSystem::getPath("resources/textures/grass.png").c_str(), "texture1", Plane::DIFFUSE);

    transparentWindow = new TransparentVerticalFace("scenes_vertex.glsl", "scenes_frag.glsl");
    transparentWindow->setupVertices();
    windowTexture = transparentWindow->loadMipMap(FileSystem::getPath("resources/textures/window.png").c_str(), "texture1", Plane::DIFFUSE);

    frameBuffer = new FrameBuffer("framebuffer_vertex.glsl", "framebuffer_frag.glsl");
    frameBuffer->setupVerticesSmallWindow();
    frameBuffer->initFrameBuffer(SCR_WIDTH, SCR_HEIGHT);

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

void drawMirror()
{
    glm::mat4 model = glm::mat4(1.0f);
    
    camera.Yaw   += 180.0f;
    // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)
    camera.ProcessMouseMovement(0, 0, false);
    glm::mat4 view = camera.GetViewMatrix();
    // reset it back to its original orientation
    camera.Yaw   -= 180.0f;
    camera.ProcessMouseMovement(0, 0, true);
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::vec3 offset = glm::vec3(0.01f);

    plane->bindTexture(GL_TEXTURE0, planeTexture);
    plane->draw(model, view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cube->bindTexture(GL_TEXTURE0, cubeTexture);
    cube->draw(fixDepthFighting(model, offset), view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->draw(fixDepthFighting(model, offset), view, projection);

    for (unsigned int i = 0; i < vegetationPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetationPositions[i]);
        vegetation->bindTexture(GL_TEXTURE0, vegetationTexture);
        vegetation->draw(fixDepthFighting(model, offset), view, projection);
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
        transparentWindow->bindTexture(GL_TEXTURE0, windowTexture);
        transparentWindow->draw(fixDepthFighting(model, offset), view, projection);
    }
}

void drawScenes()
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::vec3 offset = glm::vec3(0.01f);
    glm::mat4 model = glm::mat4(1.0f);

    plane->bindTexture(GL_TEXTURE0, planeTexture);
    plane->draw(model, view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cube->bindTexture(GL_TEXTURE0, cubeTexture);
    cube->draw(fixDepthFighting(model, offset), view, projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cube->draw(fixDepthFighting(model, offset), view, projection);

    for (unsigned int i = 0; i < vegetationPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetationPositions[i]);
        vegetation->bindTexture(GL_TEXTURE0, vegetationTexture);
        vegetation->draw(fixDepthFighting(model, offset), view, projection);
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
        transparentWindow->bindTexture(GL_TEXTURE0, windowTexture);
        transparentWindow->draw(fixDepthFighting(model, offset), view, projection);
    }
}

void loopFunc()
{
    glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
    frameBuffer->draw(false, drawMirror, bgColor, drawScenes);
}