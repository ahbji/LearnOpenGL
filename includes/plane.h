#include <learnopengl/shader.h>

#ifndef FLOOR_H
#define FLOOR_H
class Plane
{
public:
    Shader* shader;
    enum MIPMAP_TYPE
    {
        DIFFUSE,
        SPECULAR
    };
    
    Plane(const char* vertexPath, const char* fragmentPath);
    Plane(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
    void setupVertices();
    
    unsigned int loadMipMap(const char* texturePath, const std::string& textureUnitVariableName ,unsigned int textureUnitID);
    void bindTexture(GLenum textureUnit, unsigned int texture);

    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    ~Plane();
private:
    unsigned int VBO, VAO;
};
#endif
