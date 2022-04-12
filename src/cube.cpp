#include <glad/glad.h>
#include <stb_image.h>

#include <learnopengl/filesystem.h>
#include <cube.h>

Cube::Cube(const char* vertexPath, const char* fragmentPath)
{
    shader = new Shader(vertexPath, fragmentPath);
}

Cube::Cube(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    shader = new Shader(vertexPath, fragmentPath, geometryPath);
}

void Cube::setupVertices()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Cube::initTexture(
    const std::string& texturePath, 
    unsigned int* texture, 
    const std::string& textureUnitVariableName ,
    unsigned int textureUnitID,
    GLenum format)
{
    // load and create a texture 
    // -------------------------
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath(texturePath).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader->use();
    shader->setInt(textureUnitVariableName, textureUnitID);
}

unsigned int Cube::loadMipMap(const std::string& texturePath, const std::string& textureUnitVariableName ,unsigned int textureUnitID)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(FileSystem::getPath(texturePath).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        shader->use();
        shader->setInt(textureUnitVariableName, textureUnitID);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << FileSystem::getPath(texturePath).c_str() << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

void Cube::bindTexture(GLenum textureUnit, unsigned int texture) {
    // bind textures on corresponding texture units
    
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Cube::drawCube(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    // activate shader
    shader->use();
    shader->setMat4("projection", projection);
    // camera/view transformation
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // render boxes
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::setupMaterial(float shininess)
{
    shader->use();
    shader->setFloat("material.shininess", shininess);
}

void Cube::setupMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    shader->use();
    shader->setVec3("material.ambient", ambient);
    shader->setVec3("material.diffuse", diffuse);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}

void Cube::initDirectionalLight(
    glm::vec3 viewPos, 
    glm::vec3 lightDirection, 
    glm::vec3 lightAmbient, 
    glm::vec3 lightDiffuse, 
    glm::vec3 lightSpecular)
{
    shader->use();
    shader->setVec3("viewPos", viewPos);
    shader->setVec3("dirLight.direction", lightDirection);
    shader->setVec3("dirLight.ambient", lightAmbient);
    shader->setVec3("dirLight.diffuse", lightDiffuse);
    shader->setVec3("dirLight.specular", lightSpecular);
}

void Cube::initPointLight(
    unsigned int lightID, 
    glm::vec3 viewPos, 
    glm::vec3 lightPos, 
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    float constant, 
    float linear, 
    float quadratic)
{
    shader->use();
    shader->setVec3("viewPos", viewPos);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].position", lightPos);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].ambient", ambient);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].diffuse", diffuse);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].specular", specular);
    shader->setFloat("pointLights[" + std::to_string(lightID) + "].constant", constant);
    shader->setFloat("pointLights[" + std::to_string(lightID) + "].linear", linear);
    shader->setFloat("pointLights[" + std::to_string(lightID) + "].quadratic", quadratic);
}

void Cube::initSpotLight(
    unsigned int lightID, 
    glm::vec3 viewPos, 
    glm::vec3 lightPos, 
    glm::vec3 lightDirection, 
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular, 
    float constant, 
    float linear, 
    float quadratic, 
    float cutOff, 
    float outerCutOff)
{
    shader->use();
    shader->setVec3("viewPos", viewPos);
    shader->setVec3("spotLights[" + std::to_string(lightID) + "].position", lightPos);
    shader->setVec3("spotLights[" + std::to_string(lightID) + "].direction", lightDirection);
    shader->setVec3("spotLights[" + std::to_string(lightID) + "].ambient", ambient);
    shader->setVec3("spotLights[" + std::to_string(lightID) + "].diffuse", diffuse);
    shader->setVec3("spotLights[" + std::to_string(lightID) + "].specular", specular);
    shader->setFloat("spotLights[" + std::to_string(lightID) + "].constant", constant);
    shader->setFloat("spotLights[" + std::to_string(lightID) + "].linear", linear);
    shader->setFloat("spotLights[" + std::to_string(lightID) + "].quadratic", quadratic);
    shader->setFloat("spotLights[" + std::to_string(lightID) + "].cutOff", cutOff);
    shader->setFloat("spotLights[" + std::to_string(lightID) + "].outerCutOff", outerCutOff);
}

Cube::~Cube()
{
    delete shader;
    shader = nullptr;
}
