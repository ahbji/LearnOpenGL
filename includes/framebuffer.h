#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <learnopengl/shader.h>

class FrameBuffer
{
public:
    Shader* shader;
    
    FrameBuffer(const char* vertexPath, const char* fragmentPath);
    void setupVertices();
    void setupVerticesSmallWindow();
    void initFrameBuffer(unsigned int width, unsigned int height);
    void initMultisampleFrameBuffer(unsigned int width, unsigned int height);
    void draw(void (*mainScene)(), glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), bool polygonMode = false, void (*anotherScene)()  = nullptr);
    void drawMultisample(unsigned int width, unsigned int height, void (*mainScene)(), glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), bool polygonMode = false, void (*anotherScene)()  = nullptr);
    
    ~FrameBuffer();
private:
    unsigned int VBO, VAO;
    unsigned int FBO;
    unsigned int intermediateFBO;
    unsigned int textureColorbuffer;
    unsigned int textureColorBufferMultiSampled;
    unsigned int RBO;
};
#endif