#ifndef TRANSPARENT_PLANE_H
#define TRANSPARENT_PLANE_H

#include <learnopengl/shader.h>

class TransparentVerticalFace
{
public:
    Shader* shader;
    enum MIPMAP_TYPE
    {
        DIFFUSE,
        SPECULAR
    };
    
    TransparentVerticalFace(const char* vertexPath, const char* fragmentPath);
    TransparentVerticalFace(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
    void setupVertices();
    
    unsigned int loadMipMap(const char* texturePath, const std::string& textureUnitVariableName ,unsigned int textureUnitID);
    void bindTexture(GLenum textureUnit, unsigned int texture);

    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    ~TransparentVerticalFace();
private:
    unsigned int VBO, VAO;
};
#endif
