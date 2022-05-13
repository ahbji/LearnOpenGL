#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader.h>

#include <game_window.h>
#include <lighting.h>
#include <cube.h>

Shader *modelShader, *lightSrcShader;
Model *nanosuit;

Cube *lightSrcCube;
const float lightSrcSensitivity = SENSITIVITY * 4;

void loopFunc();

#define NUM_POINT_LIGHTS 4
// positions of the point lights
glm::vec3 pointLightPositions[NUM_POINT_LIGHTS] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(0.2f, 0.2f, 0.6f),
    glm::vec3(0.3f, 0.3f, 0.7f),
    glm::vec3(0.0f, 0.0f, 0.3f),
    glm::vec3(0.4f, 0.4f, 0.4f)
};
#define DEFAULT_SPOT_LIGHT_ID 0

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    modelShader = new Shader("model_lighting_vertex.glsl", "model_lighting_frag.glsl");
    nanosuit = new Model(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

    // backpack model 需要反转贴图 Y 轴
    // stbi_set_flip_vertically_on_load(true);
    // nanosuit = new Model(FileSystem::getPath("resources/objects/backpack/backpack.obj"));
    // stbi_set_flip_vertically_on_load(false);
    
    lightSrcShader = new Shader("light_src_cube_vertex.glsl", "light_src_cube_frag.glsl");
    lightSrcCube = new Cube();

    mainLoop(loopFunc);

    delete modelShader;
    delete nanosuit;
    delete lightSrcCube;
    modelShader = nullptr;
    nanosuit = nullptr;
    lightSrcCube = nullptr;
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Dirrect light
    float radius = 5.0f;
    float camX = static_cast<float>(sin(glfwGetTime() * lightSrcSensitivity) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime() * lightSrcSensitivity) * radius);
    glm::vec3 dirLightPos(camX, 0.0f, camZ);
    glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.1f);
    glm::vec3 diffuseColor = glm::vec3(0.2f, 0.2f, 0.7f);
    glm::vec3 specularColor = glm::vec3(0.7f, 0.7f, 0.7f);

    glm::vec3 spotLightAmbient = glm::vec3(0.0f);
    glm::vec3 spotLightDiffuse = glm::vec3(1.0f);
    glm::vec3 spotLightSpecular = glm::vec3(1.0f);
	
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f));

    Material* material = new Material(32.0f);
    material->applyLightingMapMaterial(modelShader);
    delete material;
    material = nullptr;

    DirectionalLight* directionalLight = new DirectionalLight(
        camera.Position,
        ambientColor, 
        diffuseColor,
        specularColor
    );
    directionalLight->apply(modelShader, -glm::normalize(dirLightPos));
    delete directionalLight;
    directionalLight = nullptr;

    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        PointLight* pointLight = new PointLight(
            camera.Position,
            pointLightColors[i] * 0.1f,
            pointLightColors[i],
            pointLightColors[i],
            constant,
            linear,
            quadratic
        );
        pointLight->apply(modelShader, i, pointLightPositions[i]);
        delete pointLight;
        pointLight = nullptr;
    }

    SpotLight* spotLight = new SpotLight(
        camera.Position,
        spotLightAmbient,
        spotLightDiffuse,
        spotLightSpecular,
        constant,
        linear,
        quadratic,
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(15.0f))
    );
    spotLight->apply(modelShader, DEFAULT_SPOT_LIGHT_ID, camera.Position, camera.Front);
    delete spotLight;
    spotLight = nullptr;

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setMat4("model", model);

    modelShader->use();
    nanosuit->Draw(*modelShader);

    // point light cubes
    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSrcCube->draw(lightSrcShader, model, view, projection);
    }

    // directional light cube
    model = glm::mat4(1.0f);
    model = glm::translate(model, dirLightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightSrcCube->draw(lightSrcShader, model, view, projection);
}