#include "FloatSlider.h"

//FloatSlider::FloatSlider()
//    :inputTextBox(0, 0, 0, 0,0,"")
//{
//
//}

FloatSlider::FloatSlider(float x, float y, float width, float height, sf::Color backgroundColor, sf::Color outlineColor, sf::Color textColor, sf::Color activeBackgroundColor, sf::Color activeTextColor)
    :backgroundColor(backgroundColor), outlineColor(outlineColor),
    textColor(textColor), activeBackgroundColor(activeBackgroundColor),
    outBox(sf::Vector2f(width, height)),
    text(x + width / 2, y, (int)(height * .75f), "0.00"), inputTextBox(x, y, width, height, (int)(height * 0.75f), "", backgroundColor, textColor, activeBackgroundColor, activeTextColor)
{
    this->type = GUIObject;

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->value = 0.0f;
    this->clickValue = 0.0f;
    this->clickX = 0;
    text.setAlign(ALIGN_CENTER);
    text.setColor(textColor);

    outBox.setPosition(x, y);

    outBox.setFillColor(backgroundColor);
    outBox.setOutlineColor(outlineColor);
    outBox.setOutlineThickness(1);

    widthValue = 0.1f;
}

void FloatSlider::onClick()
{
    if (textBoxOpen)
    {
        inputTextBox.onClick();
    }
    else
    {
        auto mouse = sf::Mouse::getPosition(*window);
        clickX = mouse.x;
        clickValue = value;
        setActive();

        int i = -2;

        while (std::pow(10, i) < std::abs(value))
        {
            i++;
        }
        widthValue = (float)std::pow(10, i);
    }
}

void FloatSlider::setUnhover()
{
    outBox.setFillColor(backgroundColor);
}

void FloatSlider::setHoverstate()
{
    outBox.setFillColor(activeBackgroundColor);
}

void FloatSlider::hoverDraw()
{
    outBox.setFillColor(activeBackgroundColor);
    draw();
}

void FloatSlider::render()
{
    update();
    draw();
}


void FloatSlider::update()
{
    if (textBoxOpen)
    {
        active = inputTextBox.getActive();
    }
    else if (active)
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (mouseMoved)
            {
                setInactive();
                textBoxOpen = false;
                activeOverride = true;
            }
            else
            {
                std::cout << "Click";
                textBoxOpen = true;
                activeOverride = true;
                inputTextBox.onClick();
                inputTextBox.setActive();
                inputTextBox.setValue(value);
                //select all
                inputTextBox.keyboardInput(1);
            }
        }
    }

    std::string textString = GUIUtilities::floatToString(value, 3);

    text.setText(textString);
}

void FloatSlider::draw()
{
    window->draw(outBox);
    text.draw();
}

void FloatSlider::activeDraw()
{
    if (textBoxOpen)
    {
        inputTextBox.activeDraw();
    }
    else
    {
        auto mouse = sf::Mouse::getPosition(*window);
        int xDifference = mouse.x - clickX;

        if (xDifference != 0)
            mouseMoved = true;
        else
            mouseMoved = false;

        float factor = xDifference / width;

        value = clickValue + factor * widthValue;

        draw();
    }
    if (!this->active)
    {
        setActive();
    }
}

void FloatSlider::setActive()
{
    active = true;
    outBox.setFillColor(activeBackgroundColor);
}

void FloatSlider::setInactive()
{
    active = false;
    if (textBoxOpen)
    {
        textBoxOpen = false;
        activeOverride = false;
        inputTextBox.setInactive();

        auto textValue = inputTextBox.getValue();
        setValue(textValue);
    }
    outBox.setFillColor(backgroundColor);
    text.setColor(textColor);
}
