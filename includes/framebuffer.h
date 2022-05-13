#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <learnopengl/shader.h>

/**
 * @brief 全屏 FrameBuffer
 * 
 */
class FrameBuffer
{
public:

    /**
     * @brief 构造普通 FrameBuffer
     * 
     * @param screenWidth 当前窗口宽度
     * @param screenHeight 当前窗口高度
     */
    FrameBuffer(unsigned int screenWidth, unsigned int screenHeight);
    
    /**
     * @brief 绘制场景到 framebuffer
     * 
     * @param shader framebuffer shader
     * @param mainScene 主场景
     * @param bgColor 线框背景色
     * @param polygonMode 是否开启线框模式，默认关闭
     * @param anotherScene 次要场景
     */
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

/**
 * @brief 小窗 FrameBuffer
 * 
 */
class SmallScreenFrameBuffer : public FrameBuffer
{
public:
    SmallScreenFrameBuffer(unsigned int screenWidth, unsigned int screenHeight);
    ~SmallScreenFrameBuffer();
};

/**
 * @brief 支持抗锯齿的 FrameBuffer
 * 
 */
class MultisampleFrameBuffer : public FrameBuffer
{
public:
    /**
     * @brief Construct a new Multisample Frame Buffer object
     * 
     * @param screenWidth 当前窗口宽度
     * @param screenHeight 当前窗口高度
     * @param fullScreen 默认为全屏模式，false 为小窗模式
     */
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