#include <learnopengl/shader.h>
#include <vector>

#ifndef CUBEMAPS_H
#define CUBEMAPS_H
class CubeMaps
{
public:
    Shader* shader;
    
    CubeMaps(const char* vertexPath, const char* fragmentPath);
    void setupVertices();
    
    unsigned int loadCubemap(std::vector<std::string> faces, const std::string& textureUnitVariableName ,unsigned int textureUnitID);
    void bindTexture(GLenum textureUnit, unsigned int textureID);

    void draw(glm::mat4 view, glm::mat4 projection);

    ~CubeMaps();
private:
    unsigned int VBO, VAO;
};
#endif
