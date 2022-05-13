#include <glad/glad.h>
#include <stb_image.h>

#include <framebuffer.h>

FrameBuffer::FrameBuffer(unsigned int screenWidth, unsigned int screenHeight)
{
    setupFullScreenVertices();
    initFrameBuffer(screenWidth, screenHeight);
}

void FrameBuffer::setupFullScreenVertices()
{
    float vertices[] = {
        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f, // top left
        -1.0f, -1.0f,  0.0f, 0.0f, // bottom left
         1.0f, -1.0f,  1.0f, 0.0f, // bottom right

        -1.0f,  1.0f,  0.0f, 1.0f, // top left
         1.0f, -1.0f,  1.0f, 0.0f, // bottom right
         1.0f,  1.0f,  1.0f, 1.0f  // top right
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

void FrameBuffer::setupSmallScreenVertices()
{
    float vertices[] = {
        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -0.3f,  1.0f,  0.0f, 1.0f, // top left
        -0.3f,  0.7f,  0.0f, 0.0f, // bottom left
         0.3f,  0.7f,  1.0f, 0.0f, // bottom right

        -0.3f,  1.0f,  0.0f, 1.0f, // top left
         0.3f,  0.7f,  1.0f, 0.0f, // bottom right
         0.3f,  1.0f,  1.0f, 1.0f  // top right
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

void FrameBuffer::initFrameBuffer(unsigned int screenWidth, unsigned int screenHeight)
{
    // framebuffer configuration
    // -------------------------
    // 创建 Framebuffer Object
    glGenFramebuffers(1, &FBO);
    // 绑定 FBO 到 GL_FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    // create a color attachment texture
    // 创建纹理对象
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    // 设置纹理对象的大小为指定的尺寸（例如窗口尺寸）
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 将纹理对象附加到 GL_FRAMEBUFFER
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    // ------------------------------------------------------------------------------------------
    // 创建 Renderbuffer Object
    glGenRenderbuffers(1, &RBO);
    // 绑定 RBO 到 GL_RENDERBUFFER
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    // 绑定回默认 RBO
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // 将 RBO 附加到 GL_FRAMEBUFFER
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    // 绑定回默认 FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setupScreenTextureUnit(Shader* shader)
{
    shader->use();
    shader->setInt("screenTexture", 0);
}

void FrameBuffer::draw(Shader* shader, void (*mainScene)(), glm::vec4 bgColor, bool polygonMode, void (*anotherScene)())
{
    // bind to framebuffer and draw scene as we normally would to color texture 
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    // enable depth testing (is disabled for rendering screen-space quad)
    glEnable(GL_DEPTH_TEST);
    // render
    // ------
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainScene();

    glDisable(GL_DEPTH_TEST);
    
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 线框模式
    if (polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnable(GL_DEPTH_TEST);
    // clear all relevant buffers
    // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (anotherScene != nullptr)
    {
        anotherScene();
    }

    shader->use();
    glBindVertexArray(VAO);
    // Draw mirror
    // use the color attachment texture as the texture of the quad plane
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &screenTexture);
}

MirrorFrameBuffer::MirrorFrameBuffer(unsigned int screenWidth, unsigned int screenHeight)
{
    setupSmallScreenVertices();
    initFrameBuffer(screenWidth, screenHeight);
}

MirrorFrameBuffer::~MirrorFrameBuffer()
{
    FrameBuffer::~FrameBuffer();
}

MultisampleFrameBuffer::MultisampleFrameBuffer(unsigned int screenWidth, unsigned int screenHeight, bool fullScreen)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    fullScreen ? setupFullScreenVertices() : setupSmallScreenVertices();
    initFrameBuffer(screenWidth, screenHeight);
}

MultisampleFrameBuffer::~MultisampleFrameBuffer()
{
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteFramebuffers(1, &intermediateFBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureColorBufferMultiSampled);
    glDeleteTextures(1, &screenTexture);
}

void MultisampleFrameBuffer::initFrameBuffer(unsigned int width, unsigned int height)
{
    // framebuffer configuration
    // -------------------------
    // 创建 Framebuffer Object
    glGenFramebuffers(1, &FBO);
    // 绑定 FBO 到 GL_FRAMEBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    // 创建纹理对象
    glGenTextures(1, &textureColorBufferMultiSampled);
    // 将纹理对象绑定到 多重采样纹理
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    // 如果最后一个参数为 GL_TRUE ，图像将会对每个纹素使用相同的样本位置以及相同数量的子采样点个数。
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    // 将多重采样纹理附加到 GL_FRAMEBUFFER
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    // ------------------------------------------------------------------------------------------
    // 创建 Renderbuffer Object
    glGenRenderbuffers(1, &RBO);
    // 绑定 RBO 到 GL_RENDERBUFFER
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    // 为 RBO 分配多重采样缓冲区
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    // 绑定回默认 RBO
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // 将 RBO 附加到 GL_FRAMEBUFFER
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    // create a color attachment texture
    // 创建纹理对象
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    // 设置纹理对象的大小为指定的尺寸（例如窗口尺寸）
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 将纹理对象附加到 GL_FRAMEBUFFER
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    // 绑定回默认 FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultisampleFrameBuffer::draw(Shader* shader, void (*mainScene)(), glm::vec4 bgColor, bool polygonMode, void (*anotherScene)())
{
    // bind to framebuffer and draw scene as we normally would to color texture 
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    // enable depth testing (is disabled for rendering screen-space quad)
    glEnable(GL_DEPTH_TEST);
    // render
    // ------
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainScene();

    // now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    glDisable(GL_DEPTH_TEST);

    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 线框模式
    if (polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnable(GL_DEPTH_TEST);
    // clear all relevant buffers
    // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (anotherScene != nullptr)
    {
        anotherScene();
    }

    shader->use();
    glBindVertexArray(VAO);
    // Draw mirror
    // use the color attachment texture as the texture of the quad plane
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}