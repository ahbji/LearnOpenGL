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

int main()
{
    window = initWindow("LearnOpenGL");
    if (window == nullptr)
    {
        return -1;
    }

    materialCube = new Cube("lighting_map_cube_vertex.glsl", "lighting_map_cube_frag.glsl");
    materialCube->setupVertices();
    diffuseMap = materialCube->loadMipMap(FileSystem::getPath("resources/textures/container2.png").c_str(), "material.diffuse", Cube::DIFFUSE);
    specularMap = materialCube->loadMipMap(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), "material.specular", Cube::SPECULAR);

    lightSrcCube = new Cube("lightSrc_cube_vertex.glsl", "lightSrc_cube_frag.glsl");
    lightSrcCube->setupVertices();

    mainLoop(loopFunc);
}

void loopFunc()
{
    // bind mipmap textures
    materialCube->bindTexture(GL_TEXTURE0, diffuseMap);
    materialCube->bindTexture(GL_TEXTURE1, specularMap);
    
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // light position
    float radius = 5.0f;
    float camX = static_cast<float>(sin(glfwGetTime() * lightSrcSensitivity) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime() * lightSrcSensitivity) * radius);
    glm::vec3 lightPos(camX, 0.0f, camZ);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    materialCube->shader->use();
    materialCube->shader->setVec3("light.position", lightPos);
    materialCube->shader->setVec3("viewPos", camera.Position);

    // light properties
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
    materialCube->shader->setVec3("light.ambient", ambientColor);
    materialCube->shader->setVec3("light.diffuse", diffuseColor);
    materialCube->shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    materialCube->shader->setFloat("material.shininess", 64.0f);

    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        materialCube->drawCube(model, view, projection);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightSrcCube->drawCube(model, view, projection);
}