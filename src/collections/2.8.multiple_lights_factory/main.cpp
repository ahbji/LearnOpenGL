#include <game_window.h>
#include <cube.h>

#include <learnopengl/filesystem.h>

Cube * materialCube;
unsigned int diffuseMap, specularMap;

Cube * lightSrcCube;

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

    materialCube = new Cube("multiple_lights_vertex.glsl", "multiple_lights_frag.glsl");
    materialCube->setupVertices();
    diffuseMap = materialCube->loadMipMap(FileSystem::getPath("resources/textures/container2.png").c_str(), "material.diffuse", Cube::DIFFUSE);
    specularMap = materialCube->loadMipMap(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), "material.specular", Cube::SPECULAR);

    lightSrcCube = new Cube("lightSrc_cube_vertex.glsl", "lightSrc_cube_frag.glsl");
    lightSrcCube->setupVertices();

    mainLoop(loopFunc);
}

void loopFunc()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind mipmap textures
    materialCube->bindTexture(GL_TEXTURE0, diffuseMap);
    materialCube->bindTexture(GL_TEXTURE1, specularMap);

    // light position
    float radius = 5.0f;
    float lightPosX = static_cast<float>(sin(glfwGetTime() * lightSrcSensitivity) * radius);
    float lightPosZ = static_cast<float>(cos(glfwGetTime() * lightSrcSensitivity) * radius);
    glm::vec3 dirLightPos(lightPosX, 0.0f, lightPosZ);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    materialCube->setupMaterial(32.0f);

    glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.1f);
    glm::vec3 diffuseColor = glm::vec3(0.2f, 0.2f, 0.7f);
    glm::vec3 specularColor = glm::vec3(0.7f, 0.7f, 0.7f);

    glm::vec3 spotLightAmbient = glm::vec3(0.0f);
    glm::vec3 spotLightDiffuse = glm::vec3(1.0f);
    glm::vec3 spotLightSpecular = glm::vec3(1.0f);
    
    materialCube->initDirectionalLight(
        camera.Position,
        -glm::normalize(dirLightPos), 
        ambientColor, 
        diffuseColor,
        specularColor
    );

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        materialCube->initPointLight(
            i,
            camera.Position,
            pointLightPositions[i],
            pointLightColors[i] * 0.1f,
            pointLightColors[i],
            pointLightColors[i],
            constant,
            linear,
            quadratic
        );
    }

    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;

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
        glm::cos(glm::radians(13.0f))
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