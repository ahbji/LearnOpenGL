#include <game_window.h>
#include <cube.h>
#include <plane.h>

#include <lighting.h>
#include <texture.h>

#include <learnopengl/filesystem.h>

using namespace learnGL;

Cube * materialCube;
Cube * lightSrcCube;
Plane * plane;

Shader* materialCubeShader, *lightSrcCubeShader, *planeShader;

Texture* diffuseMap;

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
glm::vec3 lightPos(0.0f, -2.5f, 0.0f);

// 光源颜色
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    materialCubeShader = new Shader("material_cube_vertex.glsl", "material_cube_frag.glsl");
    materialCube = new Cube();

    planeShader = new Shader("plane_vertex.glsl", "plane_frag.glsl");
    plane = new Plane();
    diffuseMap = new Texture(FileSystem::getPath("resources/textures/wood.png").c_str());
    diffuseMap->setupTextureUnit(planeShader, "material.diffuseMap", Texture::DIFFUSE);

    lightSrcCube = new Cube();
    lightSrcCubeShader = new Shader("light_src_cube_vertex.glsl", "light_src_cube_frag.glsl");

    mainLoop(loopFunc);

    delete materialCubeShader;
    delete materialCube;
    delete planeShader;
    delete plane;
    delete diffuseMap;
    delete lightSrcCube;
    delete lightSrcCubeShader;

    materialCubeShader = nullptr;
    materialCube = nullptr;
    planeShader = nullptr;
    plane = nullptr;
    diffuseMap = nullptr;
    lightSrcCube = nullptr;
    lightSrcCubeShader = nullptr;
}

void planeScene()
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

    diffuseMap->bind(GL_TEXTURE0);

    planeShader->use();
    pointLight->apply(planeShader, 0, lightPos);

    Material* material = new Material(32.0f);
    material->applyLightingMapMaterial(planeShader);

    // 清理
    delete pointLight;
    pointLight = nullptr;
    delete material;
    material = nullptr;

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
    plane->draw(planeShader, model, view, projection);
}

void cubeScene()
{
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

    // 材质属性
    glm::vec3 materialAmbient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
    glm::vec3 materialDiffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
    glm::vec3 materialSpecular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
    Material* material = new Material(materialAmbient, materialDiffuse, materialSpecular, 0.4f * 128.0f);

    materialCubeShader->use();
    // 应用光照
    pointLight->apply(materialCubeShader, 0, lightPos);
    // 应用材质
    material->applyMaterial(materialCubeShader);

    // 清理
    delete pointLight;
    pointLight = nullptr;
    delete material;
    material = nullptr;

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

        materialCube->draw(materialCubeShader, model, view, projection);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightSrcCube->draw(lightSrcCubeShader, model, view, projection);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubeScene();
    planeScene();
}