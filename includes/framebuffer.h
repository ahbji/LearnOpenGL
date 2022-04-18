#include <learnopengl/shader.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
class FrameBuffer
{
public:
    Shader* shader;
    
    FrameBuffer(const char* vertexPath, const char* fragmentPath);
    void setupVertices();
    void setupVerticesSmallWindow();
    void initFrameBuffer(unsigned int width, unsigned int height);
    void bindFrameBuffer();
    // void draw(bool polygonMode);
    // void draw(bool polygonMode, void (*anotherDraw)());
    void draw(bool polygonMode, void (*mainScene)(), glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), void (*anotherScene)()  = nullptr);

    ~FrameBuffer();
private:
    unsigned int VBO, VAO;
    unsigned int FBO;
    unsigned int textureColorbuffer;
    unsigned int RBO;
};
#endif
