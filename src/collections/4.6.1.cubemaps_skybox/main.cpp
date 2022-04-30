#include <game_window.h>
#include <cube.h>
#include <learnopengl/model.h>

#include <learnopengl/filesystem.h>
#include <cubemaps.h>

Cube * materialCube;
unsigned int diffuseMap, specularMap;

Cube * lightSrcCube;

Shader* nanosuitShader;
Model* nanosuit;

const float lightSrcSensitivity = SENSITIVITY * 4;

void loopFunc();

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  1.2f),
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

#define DEFAULT_SPOT_LIGHT_ID 0

CubeMaps* skybox;
unsigned int skyboxTexture;
vector<std::string> faces
{
    FileSystem::getPath("resources/textures/skybox/right.jpg"),
    FileSystem::getPath("resources/textures/skybox/left.jpg"),
    FileSystem::getPath("resources/textures/skybox/top.jpg"),
    FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
    FileSystem::getPath("resources/textures/skybox/front.jpg"),
    FileSystem::getPath("resources/textures/skybox/back.jpg")
};

void ininScene1()
{
    materialCube = new Cube("multiple_lights_vertex.glsl", "multiple_lights_frag.glsl");
    materialCube->setupVertices();
    diffuseMap = materialCube->loadMipMap(FileSystem::getPath("resources/textures/container2.png").c_str(), "material.diffuse", Cube::DIFFUSE);
    specularMap = materialCube->loadMipMap(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), "material.specular", Cube::SPECULAR);

    lightSrcCube = new Cube("light_src_cube_vertex.glsl", "light_src_cube_frag.glsl");
    lightSrcCube->setupVertices();
}

void initScene2()
{
    nanosuitShader = new Shader("model_vertex.glsl", "model_frag.glsl");
    nanosuit = new Model(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
}

void initSkybox()
{
    skybox = new CubeMaps("skybox_vertex.glsl", "skybox_frag.glsl");
    skybox->setupVertices();
    skyboxTexture = skybox->loadCubemap(faces, "skybox", 0);
}

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    ininScene1();
    initScene2();
    initSkybox();

    mainLoop(loopFunc);
}

void scene1()
{
    // bind mipmap textures
    materialCube->bindTexture(GL_TEXTURE0, diffuseMap);
    materialCube->bindTexture(GL_TEXTURE1, specularMap);

    // light position
    float radius = 5.0f;
    float camX = static_cast<float>(sin(glfwGetTime() * lightSrcSensitivity) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime() * lightSrcSensitivity) * radius);
    glm::vec3 dirLightPos(camX, 0.0f, camZ);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    materialCube->setupMaterial(32.0f);

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

    materialCube->initDirectionalLight(
        camera.Position,
        -glm::normalize(dirLightPos),
        ambientColor,
        diffuseColor,
        glm::vec3(1.0f, 1.0f, 1.0f)
    );

    glm::vec3 pointLightAmbient = glm::vec3(0.05f);
    glm::vec3 pointLightDiffuse = glm::vec3(0.8f);
    glm::vec3 pointLightSpecular = glm::vec3(1.0f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        materialCube->initPointLight(
            i,
            camera.Position,
            pointLightPositions[i],
            pointLightAmbient,
            pointLightDiffuse,
            pointLightSpecular,
            constant,
            linear,
            quadratic
        );
    }

    glm::vec3 spotLightAmbient = glm::vec3(0.0f);
    glm::vec3 spotLightDiffuse = glm::vec3(1.0f);
    glm::vec3 spotLightSpecular = glm::vec3(1.0f);

    materialCube->initSpotLight(
        DEFAULT_SPOT_LIGHT_ID,
        camera.Position,
        camera.Position,
        camera.Front,
        spotLightAmbient,
        spotLightDiffuse,
        spotLightSpecular,
        constant,
        linear,
        quadratic,
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(15.0f))
    );

    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        materialCube->draw(model, view, projection);
    }

    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSrcCube->draw(model, view, projection);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, dirLightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightSrcCube->draw(model, view, projection);
}

void scene2()
{
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
	
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));

    nanosuitShader->use();
    nanosuitShader->setMat4("projection", projection);
    nanosuitShader->setMat4("view", view);
    nanosuitShader->setMat4("model", model);

    nanosuit->Draw(*nanosuitShader);
}

void skyboxScene()
{
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

    skybox->bindTexture(GL_TEXTURE0, skyboxTexture);
    skybox->draw(view, projection);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skyboxScene();

    scene1();
	
    scene2();
}