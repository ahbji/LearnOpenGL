#include <game_window.h>
#include <cube.h>
#include <texture.h>
#include <lighting.h>
#include <learnOpengl/shader.h>

#include <learnopengl/filesystem.h>

using namespace learnGL;

Shader *materialCubeShader, *lightSrcCubeShader;

Cube *materialCube, *lightSrcCube;

Texture *diffuseMap, *specularMap;

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

#define NUM_POINT_LIGHTS 4
// positions of the point lights
glm::vec3 pointLightPositions[NUM_POINT_LIGHTS] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.6f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0, 0.0),
    glm::vec3(0.2f, 0.2f, 1.0f)
};

#define DEFAULT_SPOT_LIGHT_ID 0

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    materialCubeShader = new Shader("multiple_lights_vertex.glsl", "multiple_lights_frag.glsl");
    materialCube = new Cube();
    diffuseMap = new Texture(FileSystem::getPath("resources/textures/container2.png").c_str());
    diffuseMap->setupTextureUnit(materialCubeShader, "material.diffuse", Texture::DIFFUSE);
    specularMap = new Texture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
    specularMap->setupTextureUnit(materialCubeShader, "material.specular", Texture::SPECULAR);

    lightSrcCubeShader = new Shader("lightSrc_cube_vertex.glsl", "lightSrc_cube_frag.glsl");
    lightSrcCube = new Cube();

    mainLoop(loopFunc);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind mipmap textures
    diffuseMap->bind(GL_TEXTURE0);
    specularMap->bind(GL_TEXTURE1);

    // light position
    float radius = 5.0f;
    float lightPosX = static_cast<float>(sin(glfwGetTime() * lightSrcSensitivity) * radius);
    float lightPosZ = static_cast<float>(cos(glfwGetTime() * lightSrcSensitivity) * radius);
    glm::vec3 dirLightPos(lightPosX, 0.0f, lightPosZ);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    // 应用材质
    Material *material = new Material(32.0f);
    material->applyLightingMapMaterial(materialCubeShader);
    delete material;
    material = nullptr;

    glm::vec3 ambientColor = glm::vec3(0.3f, 0.24f, 0.14f);
    glm::vec3 diffuseColor = glm::vec3(0.7f, 0.42f, 0.26f);
    glm::vec3 specularColor = glm::vec3(0.5f, 0.5f, 0.5f);

    glm::vec3 spotLightAmbient = glm::vec3(0.0f);
    glm::vec3 spotLightDiffuse = glm::vec3(0.8f, 0.8f, 0.0f);
    glm::vec3 spotLightSpecular = glm::vec3(0.8f, 0.8f, 0.0f);
    
    DirectionalLight *directionalLight = new DirectionalLight(camera.Position, ambientColor, diffuseColor, specularColor);
    directionalLight->apply(materialCubeShader, -glm::normalize(dirLightPos));
    delete directionalLight;
    directionalLight = nullptr;

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        PointLight *pointLight = new PointLight(camera.Position, pointLightColors[i] * 0.1f, pointLightColors[i], pointLightColors[i], constant, linear, quadratic);
        pointLight->apply(materialCubeShader, i, pointLightPositions[i]);
        delete pointLight;
        pointLight = nullptr;
    }

    SpotLight *spotLight = new SpotLight(camera.Position, spotLightAmbient, spotLightDiffuse, spotLightSpecular, constant, linear, quadratic, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(13.0f)));
    spotLight->apply(materialCubeShader, DEFAULT_SPOT_LIGHT_ID, camera.Position, camera.Front);
    delete spotLight;
    spotLight = nullptr;

    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        materialCube->draw(materialCubeShader, model, view, projection);
    }

    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSrcCube->draw(lightSrcCubeShader, model, view, projection);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, dirLightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightSrcCube->draw(lightSrcCubeShader, model, view, projection);
}