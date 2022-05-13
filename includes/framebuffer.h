#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <learnopengl/shader.h>

class FrameBuffer
{
public:

    FrameBuffer(unsigned int screenWidth, unsigned int screenHeight);
    
    virtual void draw(Shader* shader, void (*mainScene)(), glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), bool polygonMode = false, void (*anotherScene)()  = nullptr);
    
    void setupScreenTextureUnit(Shader* shader);
    
    ~FrameBuffer();
protected:
    unsigned int VBO, VAO;
    unsigned int FBO;

    unsigned int screenTexture;
    unsigned int RBO;

    FrameBuffer() = default;
    
    virtual void initFrameBuffer(unsigned int screenWidth, unsigned int screenHeight);
    void setupFullScreenVertices();
    void setupSmallScreenVertices();
};

class MirrorFrameBuffer : public FrameBuffer
{
public:
    MirrorFrameBuffer(unsigned int screenWidth, unsigned int screenHeight);
    ~MirrorFrameBuffer();
};

class MultisampleFrameBuffer : public FrameBuffer
{
public:
    MultisampleFrameBuffer(unsigned int screenWidth, unsigned int screenHeight, bool fullScreen = true);
    virtual void draw(Shader* shader, void (*mainScene)(), glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), bool polygonMode = false, void (*anotherScene)()  = nullptr);
    ~MultisampleFrameBuffer();
private:
    unsigned int screenWidth, screenHeight;
    unsigned int intermediateFBO;
    unsigned int textureColorBufferMultiSampled;
    virtual void initFrameBuffer(unsigned int screenWidth, unsigned int screenHeight);
};
#endif