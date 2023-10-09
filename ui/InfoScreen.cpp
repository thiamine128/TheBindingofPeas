#include "InfoScreen.h"

#include "ModelManager.h"
#include "Window.h"
#include "Game.h"
#include "world/Items.h"

InfoScreen::InfoScreen(GuiRenderer* guiRenderer) : Screen(guiRenderer)
{
	this->screenType = 5;
	this->escapable = true;
	this->renderPrev = true;
	int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
	for (int i = 1; i <= Item::itemNum; ++i)
		this->guiElements.push_back(new ModelButton(i - 1, Item::m[i]->getName(), ModelManager::getModel(Item::m[i]->getModelPath()), 0.6f, [](int id) 
			{
				InfoScreen* screen = (InfoScreen*) Game::getInstance()->getScreenManager()->getTopScreen();
				screen->setItem(id + 1);
			}));

	this->item = 0;
	this->handleResize(w, h);
}

void InfoScreen::render() const
{
	
	int w = Window::getInstance()->getWidth(), h = Window::getInstance()->getHeight();
	guiRenderer->renderRect(0, 0, w, h, glm::vec4(0.1, 0.1, 0.1, 0.9));
	Screen::render();

	if (this->item)
	{
		auto desc = Item::m[this->item]->getDescription();
		for (int i = 0; i < desc.size(); ++i)
		{
			guiRenderer->renderTextCentered(w / 4 * 3, h / 6 * ( 6 - i - 1), 1.6f, desc[i]);
		}
	}
	
}

void InfoScreen::handleResize(int w, int h)
{
	for (int i = 0; i < Item::itemNum; ++i)
	{
		int row = i / 4;
		int col = i % 4;
		this->guiElements[i]->resize({ {w / 10 * col + 20, h - w / 10 * (row + 1) - 20}, {-20 + w / 10 * (col + 1), h - w / 10 * row - 20} });
	}
}

void InfoScreen::setItem(int v)
{
	this->item = v;
}
