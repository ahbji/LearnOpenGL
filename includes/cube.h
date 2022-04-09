#include <learnopengl/shader.h>

#ifndef CUBE_H
#define CUBE_H
class Cube
{
public:
    Shader* shader;
    Cube(const char* vertexPath, const char* fragmentPath);
    Cube(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
    void setupVertices();
    void initTexture(const std::string& texturePath, unsigned int* texture, const std::string& textureUnitVariableName ,unsigned int textureUnitID, GLenum format);
    void bindTexture(GLenum textureUnit, unsigned int texture);
    void drawCube(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    ~Cube();
private:
    unsigned int VBO, VAO;
};
#endif
