#pragma once

#include "Screen.h"

class ChallengeScreen
	: public Screen
{
public:
    ChallengeScreen(GuiRenderer*);

    virtual void render() const;
    virtual void handleResize(int, int);
};

