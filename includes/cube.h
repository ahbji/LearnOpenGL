#include <learnopengl/shader.h>

#ifndef CUBE_H
#define CUBE_H
class Cube
{
public:
    Shader* shader;
    enum MIPMAP_TYPE
    {
        DIFFUSE,
        SPECULAR
    };
    
    Cube(const char* vertexPath, const char* fragmentPath);
    Cube(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
    void setupVertices();
    
    unsigned int loadMipMap(const char* texturePath, const std::string& textureUnitVariableName ,unsigned int textureUnitID);

    void bindTexture(GLenum textureUnit, unsigned int texture);
    void unBindTexture();

    void setupMaterial(float shininess);
    void setupMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    void initDirectionalLight(glm::vec3 viewPos, glm::vec3 lightDirection, glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular);
    void initPointLight(unsigned int lightID, glm::vec3 viewPos, glm::vec3 lightPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
    void initSpotLight(unsigned int lightID, glm::vec3 viewPos, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
    
    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void drawWithHalo(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void drawHalo(glm::mat4 model, glm::vec3 scale, glm::mat4 view, glm::mat4 projection);

    ~Cube();
private:
    unsigned int VBO, VAO;
};
#endif
