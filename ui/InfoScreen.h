#pragma once
#include "Screen.h"
class InfoScreen :
    public Screen
{
public:
    InfoScreen(GuiRenderer*);

    virtual void render() const;
    virtual void handleResize(int, int);

    void setItem(int);
protected:
    int item;
};

