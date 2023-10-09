#include "GuiElements.h"

#include <iostream>

GuiElement::GuiElement(int id) : rect({0, 0}, {0, 0}), id(id)
{
    unfocus();
}

void GuiElement::focus(int x, int y)
{
    this->focused = true;
}

void GuiElement::unfocus()
{
    this->focused = false;
}

Rect const &GuiElement::getRect() const
{
    return this->rect;
}

void GuiElement::onClick(int x, int y)
{
}

void GuiElement::render(GuiRenderer *)
{
}

void GuiElement::onScroll(double)
{
}

void GuiElement::resize(Rect const& rect)
{
    this->rect = rect;
}

Button::Button(int id, std::string const& text, float textSize, std::function<void(int)> func) : GuiElement(id), text(text), func(func), textSize(textSize)
{

}

void Button::onClick(int x, int y)
{
    this->func(this->id);
}

void Button::render(GuiRenderer* guiRenderer)
{
    guiRenderer->renderTextCentered((rect.lb.x + rect.rt.x) >> 1, (rect.lb.y + rect.rt.y) >> 1, this->textSize + (focused ? 0.5f : 0.0f), this->text);
}

void Button::onScroll(double yOff)
{
    
}

ModelButton::ModelButton(int id, std::string const& text, const Model* model, float textSize, std::function<void(int)> callback) : Button(id, text, textSize, callback)
{
    this->model = model;
}

void ModelButton::render(GuiRenderer* guiRenderer)
{
    glm::mat4 modelMat = glm::mat4(1.0);
    modelMat = glm::scale(modelMat, glm::vec3(0.6));
    modelMat = glm::rotate(modelMat, glm::radians(22.0f), glm::vec3(0.0, 1.0, 0.0));
    //modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    guiRenderer->renderGuiModel((rect.lb.x + rect.rt.x) >> 1, rect.lb.y + 20, rect.rt.y - rect.lb.y, model, modelMat);
    guiRenderer->renderTextCentered((rect.lb.x + rect.rt.x) >> 1, rect.lb.y - 10, this->textSize + (focused ? 0.2f : 0.0f), this->text);
}

SwitchButton::SwitchButton(int id, std::string const& text, std::string const& text1, float textSize, std::function<int(void)> getter, std::function<void(int)> setter) : Button(id, text, textSize, [](int) {}), text1(text1), getter(getter), setter(setter)
{
    this->state = getter();
}

void SwitchButton::onClick(int, int)
{
    this->state = !this->state;
    setter(this->state);
}

void SwitchButton::render(GuiRenderer* guiRenderer)
{
    guiRenderer->renderTextCentered((rect.lb.x + rect.rt.x) >> 1, (rect.lb.y + rect.rt.y) >> 1, this->textSize + (focused ? 0.5f : 0.0f), this->state ? this->text1 : this->text);
}

AdjustButton::AdjustButton(int id, int minV, int maxV, std::function<int(void)> getter, std::function<void(int)> setter) : GuiElement(id), minV(minV), maxV(maxV), getter(getter), setter(setter)
{

}

void AdjustButton::onClick(int x, int y)
{
    int rx = x - rect.lb.x;
    int w = rect.rt.x - rect.lb.x;
    if (rx <= w / 3)
    {
        if (getter() <= minV)
        {
            setter(maxV);
        }
        else
        {
            setter(getter() - 1);
        }
    }
    else if (rx >= w / 3 * 2)
    {
        if (getter() >= maxV)
        {
            setter(minV);
        }
        else {
            setter(getter() + 1);
        }
    }
}

void AdjustButton::render(GuiRenderer* guiRenderer)
{
    int w = rect.rt.x - rect.lb.x;
    guiRenderer->renderTextCentered((rect.lb.x + rect.rt.x) >> 1, (rect.lb.y + rect.rt.y) >> 1, 1.0f, std::to_string(getter()));
    guiRenderer->renderTextCentered(rect.lb.x + w / 3, (rect.lb.y + rect.rt.y) >> 1, 1.0f + (focus1 ? 0.4f : 0.0f), "<");
    guiRenderer->renderTextCentered(rect.lb.x + w / 3 * 2, (rect.lb.y + rect.rt.y) >> 1, 1.0f + (focus2 ? 0.4f : 0.0f), ">");
}

void AdjustButton::focus(int x, int y)
{
    int rx = x - rect.lb.x;
    int w = rect.rt.x - rect.lb.x;
    if (rx <= w / 3)
    {
        focus1 = true;
        focus2 = false;
    }
    else if (rx >= w / 3 * 2)
    {
        focus1 = false;
        focus2 = true;
    }
    else
    {
        focus1 = focus2 = false;
    }
}

void AdjustButton::unfocus()
{
    focus1 = focus2 = false;
}
