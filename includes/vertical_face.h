#ifndef TRANSPARENT_PLANE_H
#define TRANSPARENT_PLANE_H

#include <learnopengl/shader.h>

class TransparentVerticalFace
{
public:
    TransparentVerticalFace();

    /**
     * @brief 绘制透明立面
     *
     * @param shader
     * @param model 
     * @param view
     * @param projection 
     */
    void draw(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    ~TransparentVerticalFace();
private:
    unsigned int VBO, VAO;
    void setupVertices();
};
#endif
