#ifndef CUBEMAPS_H
#define CUBEMAPS_H

#include <learnopengl/shader.h>
#include <vector>
class CubeMaps
{
public:
    CubeMaps();
    void setupVertices();

    void draw(Shader* shader, glm::mat4 view, glm::mat4 projection);

    ~CubeMaps();
private:
    unsigned int VBO, VAO;
};
#endif
