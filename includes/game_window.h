#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>


#include <iostream>

#ifndef HEADER_GAME_WINDOW_H
#define HEADER_GAME_WINDOW_H

#define GLFW_MAJOR_VERSION 3
#define GLFW_MINOR_VERSION 3

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

GLFWwindow* window;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float currentFrame = 0.0f;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow* initWindow(const char* windowName);
void mainLoop(void (* loopfunc)());
void closeWindow();

// glfw: 每当窗口大小改变(通过操作系统或用户调整大小)，这个回调函数就会执行
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// glfw: 每当鼠标移动时，这个回调函数就会被调用  
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
// glfw: 每当鼠标滚轮滚动时，这个回调函数就会被调用  
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 处理键盘输入:查询 GLFW 此帧是否按下/释放相关按键并做出相应反应 
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window);

/**
 * @brief 初始化 OpenGL window
 * 
 * @param windowName 窗口名
 * @return GLFWwindow* 返回 GLFWwindow 指针
 */
GLFWwindow* initWindow(const char* windowName)
{
    // 初始化 glfw
    glfwInit();
    // 设置 opengl 版本号，以便 glfw 创建 OpenGL 上下文时做出适当的调整，
    // 同时在当前系统没有适当的 OpenGL 版本时无法运行。
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
    // 设置 GLFW 使用 OpenGL Core 模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建 glfw window
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    // 将 window 设置为当前线程的 main context
    glfwMakeContextCurrent(window);

    // 改变窗口的大小的时候，设置回调调整 viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 设置 GLFW 捕获 mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // 初始化 GLAD ：加载所有 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST); // enable depth test

    return window;
}

/**
 * @brief 执行主循环
 * 
 * @param looperFunc 指定 looper 函数
 */
void mainLoop(void (*looperFunc)())
{
    // 检查 GLFW 是否被要求退出，如果是，则结束 Looper
    while (!glfwWindowShouldClose(window))
    {
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 处理输入控制
        processInput(window);

        (*looperFunc)();

        // 交换双缓冲
        // 当前 looper 中的渲染指令在 back buffer 中绘制图像，
        // 当前 looper 结束前，需要将 back buffer 交换到 front buffer ，使图像显示出来。
        glfwSwapBuffers(window);
        // 检查有鼠标、键盘、窗口更新等事件，并调用对应 callback
        glfwPollEvents();
    }
}

/**
 * @brief 关闭 GLFW window
 */
void closeWindow()
{   
    // 释放 GLFW 窗口
    glfwTerminate();
}

// glfw: 每当窗口大小改变时，这个回调函数就会被调用
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保 viewport 符合新窗口大小，注意，width 和 height 在 retina 屏幕上将会明显比指定的大。
    glViewport(0, 0, width, height);
}

// glfw: 每当鼠标移动时，这个回调函数就会被调用
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 因为坐标是从下向上的

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: 每当鼠标滚轮滚动时，这个回调函数就会被调用
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// 处理输入控制：查询 GLFW 是否在这一帧被按下或释放了相关的按键，并做出相应的反应
// ----------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
#endif // HEADER_APP