#include "ChallengeScreen.h"

#include "Window.h"
#include "Game.h"
#include "world/challenge/PotatoesChallenge.h"

ChallengeScreen::ChallengeScreen(GuiRenderer* guiRenderer) : Screen(guiRenderer)
{
	this->screenType = 6;
	this->renderPrev = true;
	this->escapable = true;

	int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();

	this->guiElements.push_back(new Button(0, "Potatoes", 1.0f, [](int) {Game::getInstance()->startNewChallenge(new PotatoesChallenge()); }));
	this->handleResize(w, h);
}

void ChallengeScreen::render() const
{
	int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
	glDisable(GL_DEPTH_TEST);
	guiRenderer->renderRect(0, 0, w, h, glm::vec4(0.1, 0.1, 0.1, 0.9));
	Screen::render();
}

void ChallengeScreen::handleResize(int w, int h)
{
	guiElements[0]->resize({ { w / 2 - 100, h / 2 - 50 }, {w / 2 + 100, h / 2 + 50} });
}
