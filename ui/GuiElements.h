#pragma once

#include "rendering/GuiRenderer.h"
#include "util/Rect.h"
#include <functional>

class GuiElement
{
public:
    GuiElement(int);

    Rect const& getRect() const;
    virtual void onClick(int, int);
    virtual void render(GuiRenderer*);
    virtual void onScroll(double);
    void resize(Rect const&);

    virtual void focus(int, int);
    virtual void unfocus();
protected:
    Rect rect;
    bool focused;
    int id;
};

class Button : public GuiElement
{
public:
    Button(int, std::string const&, float, std::function<void(int)>);

    virtual void onClick(int, int);
    virtual void render(GuiRenderer*);
    virtual void onScroll(double);
protected:
    std::string text;
    float textSize;
    std::function<void(int)> func;
};

class ModelButton : public Button
{
public:
    ModelButton(int, std::string const&, const Model*, float, std::function<void(int)>);

    virtual void render(GuiRenderer*);
protected:
    const Model* model;
};

class SwitchButton : public Button
{
public:
    SwitchButton(int, std::string const&, std::string const&, float, std::function<int(void)>, std::function<void(int)>);

    virtual void onClick(int, int);
    virtual void render(GuiRenderer*);
protected:
    int state;
    std::string text1;
    std::function<int(void)> getter;
    std::function<void(int)> setter;
};

class AdjustButton : public GuiElement
{
public:
    AdjustButton(int, int, int, std::function<int(void)>, std::function<void(int)>);

    virtual void onClick(int, int);
    virtual void render(GuiRenderer*);
    virtual void focus(int, int);
    virtual void unfocus();
protected:
    int minV, maxV;
    bool focus1, focus2;
    std::function<int(void)> getter;
    std::function<void(int)> setter;
};