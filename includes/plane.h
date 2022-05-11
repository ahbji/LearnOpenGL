#ifndef FLOOR_H
#define FLOOR_H

#include <learnopengl/shader.h>

class Plane
{
public:

    Plane();

    /**
     * @brief 绘制 Plane
     * 
     * @param model 
     * @param view 
     * @param projection 
     */
    void draw(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    ~Plane();
private:
    unsigned int VBO, VAO, EBO;
    /**
     * @brief 初始化顶点数据，原点坐标在图片左下角，贴图分别在X轴和Y轴上重复两次
     */
    void setupVertices();
};
#endif
