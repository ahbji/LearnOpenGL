#ifndef SHADOW_H
#define SHADOW_H

#include <glm/glm.hpp>
#include <learnopengl/shader.h>

#include <glm/gtc/matrix_transform.hpp>

class Shadow
{
public:
    Shadow(glm::vec3 lightPos, unsigned int shadowWidth = 1024, unsigned int shadowHeight = 1024, unsigned int screenWidth = 800, unsigned int screenHeight = 600);

    /**
     * @brief 
     * 
     * @param sceneShader 场景着色器
     * @param textureUnitID 
     */
    void bindDepthMapTexture(Shader* sceneShader, unsigned int textureUnitID = 5);

    /**
     * @brief 计算深度贴图，同时渲染场景。
     * 
     * @param shadowMapShader 
     * @param sceneShader 
     * @param scene 
     */
    void renderingWithShadow(Shader* shadowMapShader, Shader* sceneShader, void (*scene)(Shader* shader));
    
protected:
    /**
     * @brief 创建深度贴图的 Framebuffer
     */
    unsigned int depthMapFBO;
    /**
     * @brief 存储深度贴图的纹理
     */
    unsigned int shadowMapTexture;
    /**
     * @brief 初始化 depthMapFBO
     * 
     */
    void initDepthMapFBO();

private:
    // 光源位置参与深度贴图的计算
    glm::vec3 lightPos;
    // 记录深度贴图的分辨率
    unsigned int shadow_width, shadow_height;
    unsigned int screen_width, screen_height;
};


Shadow::Shadow(glm::vec3 lightPos, unsigned int shadowWidth, unsigned int shadowHeight, unsigned int screenWidth, unsigned int screenHeight):lightPos(lightPos), shadow_width(shadowWidth), shadow_height(shadowHeight), screen_width(screenWidth), screen_height(screenHeight)
{
    initDepthMapFBO();
}

void Shadow::initDepthMapFBO()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // 绑定 depthMapFBO 到 GL_FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    // 附加阴影贴图到 FrameBuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    // 解绑 depthMapFBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::bindDepthMapTexture(Shader* sceneShader, unsigned int textureUnitID)
{
    sceneShader->use();
    sceneShader->setInt("shadowMap", textureUnitID); // 设置阴影贴图的纹理单元
    // glActiveTexture(GL_TEXTURE0 + textureUnitID);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
}


void Shadow::renderingWithShadow(Shader* shadowMapShader, Shader* sceneShader, void (*scene)(Shader* shader))
{
    glViewport(0, 0, shadow_width, shadow_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // 1. 以光源视角获得的场景深度值渲染阴影贴图
    glm::mat4 lightProjection, lightView;
    float near_plane = 1.0f, far_plane = 7.5f;

    // 场景光照为平行光，所需创建正交投影矩阵
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    // 以光源的位置为视点构建 view 矩阵
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

    shadowMapShader->use();
    shadowMapShader->setMat4("projection", lightProjection);
    shadowMapShader->setMat4("view", lightView);
    scene(shadowMapShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. 结合阴影贴图渲染场景
    // 得到光源的 VP 矩阵
    glViewport(0, 0, screen_width, screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightSpaceMatrix;
    lightSpaceMatrix = lightProjection * lightView;
    sceneShader->use();
    // 在着色器中用来将世界空间坐标系的顶点位置转换到光空间坐标系，以判断是否渲染阴影
    sceneShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    scene(sceneShader);
}


#endif // SHADOW_H