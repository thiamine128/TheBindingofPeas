#include "Controller.h"

#include "Window.h"
#include "Game.h"
#include "rendering/WorldRenderer.h"
#include "SoundManager.h"
#include "rendering/Camera.h"
#include "util/Options.h"
#include <GLFW/glfw3.h>

#include <iostream>

Controller::Controller()
{
    this->mouseControl = true;
}

Controller::~Controller()
{
}

//根据键盘输入得到玩家的移动方向
glm::vec3 Controller::getVelocity()
{
    const float speed = 1.0;
    glm::vec3 ret = glm::vec3(0.0);
    if (this->isKeyPressed(GLFW_KEY_W))
        ret.z += speed;
    if (this->isKeyPressed(GLFW_KEY_A))
        ret.x += speed;
    if (this->isKeyPressed(GLFW_KEY_S))
        ret.z -= speed;
    if (this->isKeyPressed(GLFW_KEY_D))
        ret.x -= speed;
    return ret;
}

bool Controller::isKeyPressed(int key) const
{
    return Window::getInstance()->getKey(key) == GLFW_PRESS;
}

bool Controller::isMouseButtonPressed(int key) const
{
    return Window::getInstance()->getMouseButton(key) == GLFW_PRESS;
}

//将输入事件传递给游戏界面管理器
void Controller::onPress(int key) const
{
    if (key == GLFW_KEY_ESCAPE)
    {
        Game::getInstance()->getScreenManager()->escape();
    }
    Game::getInstance()->getScreenManager()->handleKeyPress(key);
}

void Controller::processInput()
{

}

void Controller::onMouseButtonPress(int button, double x, double y) const
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        Game::getInstance()->getScreenManager()->handleClick(x, Window::getInstance()->getHeight() - y);
    }
}

void Controller::onScroll(double xOff, double yOff) const
{
    Game::getInstance()->getScreenManager()->handleScroll(this->mouseX, Window::getInstance()->getHeight() - this->mouseY, yOff);
    Game::getInstance()->getWorldRenderer()->getCamera()->addR(-yOff);
}

void Controller::onCursorMove(double x, double y)
{
    if (glfwGetMouseButton(Window::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        Game::getInstance()->getWorldRenderer()->getCamera()->addRadian((x - this->mouseX) / Window::getInstance()->getWidth() * 2.0);
        Game::getInstance()->getWorldRenderer()->getCamera()->addPitch((y - this->mouseY) / Window::getInstance()->getHeight() * 2.0);
    }
    this->mouseX = x;
    this->mouseY = y;
    Game::getInstance()->getScreenManager()->onCursorMove(mouseX, Window::getInstance()->getHeight() - mouseY);
}

double Controller::getMouseX() const
{
    return this->mouseX;
}

double Controller::getMouseY() const
{
    return this->mouseY;
}

bool Controller::useMouseControl() const
{
    return Options::getControl();
}
