#include "MenuScreen.h"

#include "Game.h"
#include "Window.h"
#include "TextureManager.h"

MenuScreen::MenuScreen(GuiRenderer* guiRenderer) : Screen(guiRenderer)
{
    this->screenType = 3;

    int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
    this->guiElements.push_back(new Button(0, "New Game", 1.0f, [](int){Game::getInstance()->startNewGame();}));
    this->guiElements.push_back(new Button(1, "Quit", 1.0f, [](int){Window::getInstance()->close();}));
    this->guiElements.push_back(new Button(2, "Help", 1.0f, [](int){Game::getInstance()->getScreenManager()->pushHelpScreen();}));
    this->guiElements.push_back(new Button(3, "Info", 1.0f, [](int) {Game::getInstance()->getScreenManager()->pushInfoScreen(); }));
    this->guiElements.push_back(new Button(4, "Challenge", 1.0f, [](int) {Game::getInstance()->getScreenManager()->pushChallengeScreen(); }));
    this->guiElements.push_back(new Button(5, "Options", 1.0f, [](int) {Game::getInstance()->getScreenManager()->pushOptionsScreen(); }));
    this->handleResize(w, h);

    this->bg = TextureManager::getTexture("assets/textures/background.png");
}

void MenuScreen::render() const
{
    glDisable(GL_DEPTH_TEST);
    int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
    guiRenderer->renderImage(0, 0, w, h, bg);
    this->guiRenderer->renderTextCentered(w / 2, h / 6 * 5, 1.0f, "Highest Score:" + std::to_string(Game::getInstance()->getHighestScore()));
    this->guiRenderer->renderTextCentered(w / 2, h / 4 * 3, 2.0f, "The Binding of Peas");
    Screen::render();
    glEnable(GL_DEPTH_TEST);
}

void MenuScreen::handleKeyPress(int)
{

}

void MenuScreen::handleResize(int w, int h)
{
    this->guiElements[0]->resize({{w / 2 - 200, h / 2 - 50}, {w / 2, h / 2 + 50}});
    this->guiElements[1]->resize({{w / 2, h / 2 - 150}, {w / 2 + 200, h / 2 - 50}});
    this->guiElements[5]->resize({ {w / 2 - 200, h / 2 - 150}, {w / 2, h / 2 - 50} });
    this->guiElements[2]->resize({{w / 2 - 200, h / 2 - 250}, {w / 2, h / 2 - 150}});
    this->guiElements[3]->resize({{w / 2, h / 2 - 250 }, {w / 2 + 200, h / 2 - 150}});
    this->guiElements[4]->resize({ {w / 2, h / 2 - 50}, {w / 2 + 200, h / 2 + 50} });
}