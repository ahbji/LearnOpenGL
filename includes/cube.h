#ifndef CUBE_H
#define CUBE_H

#include <learnopengl/shader.h>

class Cube
{
public:
  
    Cube();

    void draw(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void drawWithHalo(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void drawHalo(Shader* shader, glm::mat4 model, glm::vec3 scale, glm::mat4 view, glm::mat4 projection);

    ~Cube();
private:
    unsigned int VBO, VAO;
    void setupVertices();
};
#endif
