#include "PauseScreen.h"

#include "Window.h"
#include "Game.h"

#include <GLFW/glfw3.h>

PauseScreen::PauseScreen(GuiRenderer* guiRenderer) : Screen(guiRenderer)
{
    this->screenType = 4;
    this->renderPrev = true;

    this->guiElements.push_back(new Button(0, "Continue", 1.0f, [](int){Game::getInstance()->continueGame();}));
    this->guiElements.push_back(new Button(1, "Menu", 1.0f, [](int) {Game::getInstance()->getScreenManager()->popScreen(); Game::getInstance()->getScreenManager()->popScreen(); Game::getInstance()->quitGame(); }));
    this->guiElements.push_back(new Button(2, "Options", 1.0f, [](int) {Game::getInstance()->getScreenManager()->pushOptionsScreen(); }));
    this->handleResize(Window::getInstance()->getWidth(), Window::getInstance()->getHeight());
}

void PauseScreen::render() const
{
    glDisable(GL_DEPTH_TEST);
    int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
    guiRenderer->renderRect(0, 0, w, h, glm::vec4(0.2));
    guiRenderer->renderTextCentered(w / 2, h - 200, 1.5f, "Game Paused");
    Screen::render();
    glEnable(GL_DEPTH_TEST);
}

void PauseScreen::handleKeyPress(int key)
{
    
}

void PauseScreen::handleResize(int w, int h)
{
    this->guiElements[0]->resize({{w / 2 - 100, h / 2 - 50}, {w / 2 + 100, h / 2 + 50}});
    this->guiElements[1]->resize({{w / 2 - 100, h / 2 - 150}, {w / 2 + 100, h / 2 - 50}});
    this->guiElements[2]->resize({ {w / 2 - 100, h / 2 - 250}, {w / 2 + 100, h / 2 - 150} });
}
