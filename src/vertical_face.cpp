#include <glad/glad.h>
#include <stb_image.h>

#include <vertical_face.h>

TransparentVerticalFace::TransparentVerticalFace(const char* vertexPath, const char* fragmentPath)
{
    shader = new Shader(vertexPath, fragmentPath);
}

TransparentVerticalFace::TransparentVerticalFace(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    shader = new Shader(vertexPath, fragmentPath, geometryPath);
}

void TransparentVerticalFace::setupVertices()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, // top-left
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f, // bottom-left
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f, // bottom-right

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, // top-left
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f, // bottom-right
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f  // top-right
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

unsigned int TransparentVerticalFace::loadMipMap(const char* texturePath, const std::string& textureUnitVariableName ,unsigned int textureUnitID)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        shader->use();
        shader->setInt(textureUnitVariableName, textureUnitID);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

void TransparentVerticalFace::bindTexture(GLenum textureUnit, unsigned int texture) {
    // bind textures on corresponding texture units
    
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void TransparentVerticalFace::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    // activate shader
    shader->use();
    shader->setMat4("projection", projection);
    // camera/view transformation
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // render boxes
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}