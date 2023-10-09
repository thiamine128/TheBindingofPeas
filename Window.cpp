#include "Window.h"

#include "Game.h"
#include "Controller.h"
#include "ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

Window* Window::instance = nullptr;

Window::Window() {

}

//初始化glfw窗口
void Window::init(int width, int height, const char* title)
{
    this->width = width;
    this->height = height;
    this->title = title;

    //窗口初始化及配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create glfw window." << std::endl;
        glfwTerminate();
        return;
    }

    //设置回调函数来处理用户输入
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
    glfwSetKeyCallback(this->window, keyCallback);
    glfwSetMouseButtonCallback(this->window, mouseButtonCallback);
    glfwSetScrollCallback(this->window, scrollCallback);
    glfwSetCursorPosCallback(this->window, cursorPosCallback);
}

//销毁窗口
Window::~Window()
{
    glfwTerminate();
}

//准备关闭窗口
bool Window::shouldClose() const
{
    return glfwWindowShouldClose(this->window);
}

//交换缓冲
void Window::swapBuffers() const
{
    glfwSwapBuffers(this->window);
}

//处理输入事件
void Window::pollEvents() const
{
    glfwPollEvents();
}

//关闭窗口
void Window::close() const
{
    glfwSetWindowShouldClose(this->window, true);
}

int Window::getKey(int key) const {
    return glfwGetKey(this->window, key);
}

int Window::getMouseButton(int key) const
{
    return glfwGetMouseButton(window, key);
}

//更新窗口大小
void Window::onResize(int width, int height)
{
    this->width = width;
    this->height = height;
}

int Window::getWidth() const
{
    return this->width;
}

int Window::getHeight() const
{
    return this->height;
}

GLFWwindow *Window::getWindow() const
{
    return this->window;
}

Window *Window::getInstance()
{
    if (instance == nullptr) {
        instance = new Window();
    }
    return instance;
}

//关闭窗口
void Window::terminate()
{
    delete instance;
}

//窗口大小发生改变，进行一些数据更新
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //opengl视图大小
    glViewport(0, 0, width, height);
    //更新储存的窗口大小
    Window::getInstance()->onResize(width, height);
    //更新渲染用的视图矩阵
    Game::getInstance()->getWorldRenderer()->updateProjection();
    Game::getInstance()->getGui()->updateViewport();
    //更新矩阵后向着色器传入新的矩阵
    ShaderManager::initUniforms(Game::getInstance()->getWorldRenderer(), Game::getInstance()->getGui());
    //界面大小改变事件，部分界面修改帧缓冲的大小
    Game::getInstance()->getScreenManager()->handleResize(width, height);
}

//控制方面的监听，传递给Controller类进行处理
void keyCallback(GLFWwindow* window, int key, int code, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        Game::getInstance()->getController()->onPress(key);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        Game::getInstance()->getController()->onMouseButtonPress(button, x, y);
    }
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Game::getInstance()->getController()->onScroll(xOffset, yOffset);
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
    Game::getInstance()->getController()->onCursorMove(x, y);
}
