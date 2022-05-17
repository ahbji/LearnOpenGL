#include <game_window.h>
#include <cube.h>
#include <plane.h>
#include <texture.h>
#include <lighting.h>
#include <shadow.h>
#include <learnOpengl/shader.h>

#include <learnopengl/filesystem.h>

// TODO 阴影渲染错了

using namespace learnGL;

Shader *sceneShader, *shadowMapShader;

Cube *materialCube, *lightSrcCube;
Plane *plane;

Shadow *shadow;

Texture *materialCubeDiffuseMap, *materialCubeSpecularMap, *planeDiffuseMap;

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

#define DEFAULT_POINT_LIGHT_ID 0

// 光源位置
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

// 光源颜色
glm::vec3 lightColor(1.0f);

#define ID_MATERIAL_CUBE 0
#define ID_PLANE 1
#define ID_LIGHT_SRC_CUBE 2

void planeLighting(Shader *shader)
{
    // 点光源
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
    glm::vec3 specularColor = lightColor;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    PointLight* pointLight = new PointLight(
        camera.Position,
        ambientColor,
        diffuseColor,
        specularColor,
        constant,
        linear,
        quadratic
    );

    planeDiffuseMap->bind(GL_TEXTURE0);

    pointLight->apply(shader, 0, lightPos);

    Material* material = new Material(32.0f);
    material->applyLightingMapMaterial(shader);

    // 清理
    delete pointLight;
    pointLight = nullptr;
    delete material;
    material = nullptr;
}

void planeScene(Shader *shader)
{
    shader->use();
    shader->setInt("sceneID", ID_PLANE);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
    plane->draw(shader, model, view, projection);
}

void cubeLighting(Shader *shader)
{
    // bind mipmap textures
    materialCubeDiffuseMap->bind(GL_TEXTURE0);
    materialCubeSpecularMap->bind(GL_TEXTURE1);

    // 基础光照属性
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
    glm::vec3 specularColor = lightColor;

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    PointLight* pointLight = new PointLight(
        camera.Position,
        ambientColor,
        diffuseColor,
        specularColor,
        constant,
        linear,
        quadratic
    );


    Material *material = new Material(0.4f * 128.0f);

    // 应用光照
    pointLight->apply(shader, 0, lightPos);
    // 应用材质
    material->applyLightingMapMaterial(shader);

    // 清理
    delete pointLight;
    pointLight = nullptr;
    delete material;
    material = nullptr;
}

void cubesScene(Shader *shader)
{
    shader->use();
    shader->setInt("sceneID", ID_MATERIAL_CUBE);
    
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

        materialCube->draw(shader, model, view, projection);
    }

    // shader->use();
    // shader->setInt("sceneID", ID_LIGHT_SRC_CUBE);

    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, lightPos);
    // model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    // lightSrcCube->draw(shader, model, view, projection);
}

void scene(Shader *shader)
{
    cubeLighting(sceneShader);
    cubesScene(sceneShader);
    planeLighting(sceneShader);
    planeScene(sceneShader);
}

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    sceneShader = new Shader("scenes_vertex.glsl", "scenes_frag.glsl");
    shadowMapShader = new Shader("dir_shadow_map.vs", "dir_shadow_map.fs");

    materialCube = new Cube();
    materialCubeDiffuseMap = new Texture(FileSystem::getPath("resources/textures/container2.png").c_str());
    materialCubeDiffuseMap->setupTextureUnit(sceneShader, "material.diffuse", Texture::DIFFUSE);
    materialCubeSpecularMap = new Texture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
    materialCubeSpecularMap->setupTextureUnit(sceneShader, "material.specular", Texture::SPECULAR);

    plane = new Plane();
    planeDiffuseMap = new Texture(FileSystem::getPath("resources/textures/wood.png").c_str());
    planeDiffuseMap->setupTextureUnit(sceneShader, "material.diffuseMap", Texture::DIFFUSE);

    lightSrcCube = new Cube();

    shadow = new Shadow(lightPos);
    shadow->bindDepthMapTexture(sceneShader);

    mainLoop(loopFunc);

    delete materialCube;
    delete plane;
    delete planeDiffuseMap;
    delete materialCubeDiffuseMap;
    delete materialCubeSpecularMap;
    delete lightSrcCube;

    delete sceneShader;

    materialCube = nullptr;
    plane = nullptr;
    planeDiffuseMap = nullptr;
    materialCubeDiffuseMap = nullptr;
    materialCubeSpecularMap = nullptr;
    lightSrcCube = nullptr;

    sceneShader = nullptr;
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // scene(sceneShader);
    shadow->renderingWithShadow(shadowMapShader, sceneShader, scene);
}