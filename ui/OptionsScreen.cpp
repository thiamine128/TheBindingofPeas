#include "OptionsScreen.h"

#include "util/Options.h"
#include "SoundManager.h"
#include "Window.h"

OptionsScreen::OptionsScreen(GuiRenderer* guiRenderer) : Screen(guiRenderer)
{
	this->screenType = 7;
	this->escapable = true;
	this->renderPrev = true;

	this->guiElements.push_back(new SwitchButton(0, "Keyboard", "Mouse", 1.0f, Options::getControl, Options::setControl));
	this->guiElements.push_back(new SwitchButton(1, "Off", "On", 1.0f, Options::getDebug, Options::setDebug));
	this->guiElements.push_back(new AdjustButton(2, 0, 10, Options::getSFX, Options::setSFX));
	this->guiElements.push_back(new AdjustButton(2, 0, 10, Options::getMusic, Options::setMusic));

	int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
	this->handleResize(w, h);
}

void OptionsScreen::render() const
{
	glDisable(GL_DEPTH_TEST);
	int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
	guiRenderer->renderRect(0, 0, w, h, glm::vec4(0.2, 0.2, 0.2, 0.9));
	guiRenderer->renderTextCentered(w / 2 - 150, h / 6 * 5, 1.0f, "Control:");
	guiRenderer->renderTextCentered(w / 2 - 150, h / 6 * 4, 1.0f, "Debug:");
	guiRenderer->renderTextCentered(w / 2 - 150, h / 6 * 3, 1.0f, "SFX:");
	guiRenderer->renderTextCentered(w / 2 - 150, h / 6 * 2, 1.0f, "Music:");
	Screen::render();
	glEnable(GL_DEPTH_TEST);
}

void OptionsScreen::handleKeyPress(int)
{
}

void OptionsScreen::handleResize(int w, int h)
{
	this->guiElements[0]->resize({ {w / 2 + 100, h / 6 * 5 - 50}, {w / 2 + 200, h / 6 * 5 + 50} });
	this->guiElements[1]->resize({ {w / 2 + 100, h / 6 * 4 - 50}, {w / 2 + 200, h / 6 * 4 + 50} });
	this->guiElements[2]->resize({ {w / 2 + 50, h / 6 * 3 - 50}, {w / 2 + 250, h / 6 * 3 + 50} });
	this->guiElements[3]->resize({ {w / 2 + 50, h / 6 * 2 - 50}, {w / 2 + 250, h / 6 * 2 + 50} });
}

void OptionsScreen::onPop()
{
	Options::save();
}
