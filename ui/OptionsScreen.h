#pragma once

#include "Screen.h"

class OptionsScreen
	: public Screen
{
public:
	OptionsScreen(GuiRenderer*);

	virtual void render() const;
	virtual void handleKeyPress(int);
	virtual void handleResize(int, int);
	virtual void onPop();
};

