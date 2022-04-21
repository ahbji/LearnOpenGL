#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader.h>

#include <game_window.h>
#include <model_lighting.h>
#include <cube.h>

Shader* shader;
Model* nanosuit;

Cube * lightSrcCube;
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

    shader = new Shader("model_vertex.glsl", "model_frag.glsl");
    nanosuit = new Model(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

    mainLoop(loopFunc);
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

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    
    shader->use();
    ModelLighting::setupMaterial(shader, 32.0f);
    ModelLighting::initDirectionalLight(
        shader,
        camera.Position,
        -glm::normalize(dirLightPos), 
        ambientColor, 
        diffuseColor,
        specularColor
    );
    for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        ModelLighting::initPointLight(
            shader,
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
    ModelLighting::initSpotLight(
		shader,
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
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);

    nanosuit->Draw(*shader);
}