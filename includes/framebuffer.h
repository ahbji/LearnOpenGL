#include <learnopengl/shader.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
class FrameBuffer
{
public:
    Shader* shader;
    
    FrameBuffer(const char* vertexPath, const char* fragmentPath);
    void setupVertices();
    void initFrameBuffer(unsigned int width, unsigned int height);
    void bindFrameBuffer();
    void draw(bool polygonMode);

    ~FrameBuffer();
private:
    unsigned int VBO, VAO;
    unsigned int FBO;
    unsigned int textureColorbuffer;
    unsigned int RBO;
};
#endif
