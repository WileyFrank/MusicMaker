#include "BoundedFloatSlider.h"

BoundedFloatSlider::BoundedFloatSlider(float x, float y, float width, float height, sf::Color backgroundColor, sf::Color outlineColor, sf::Color fillColor, sf::Color textColor, sf::Color activeBackgroundColor, sf::Color activeFillColor, sf::Color activeTextColor)
    :fillColor(fillColor), activeFillColor(activeFillColor),
    FloatSlider(x, y, width, height, backgroundColor, outlineColor, textColor, activeBackgroundColor, activeTextColor)
{
    this->backgroundColor = backgroundColor;
    this->outlineColor = outlineColor;
    this->textColor = textColor;
    this->activeBackgroundColor = activeBackgroundColor;

    this->type = GUIObject;

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->minValue = 0.0f;
    this->maxValue = 1.0f;
    this->value = 0.50f;
    this->clickValue = 0.0f;
    this->clickX = 0;
    text.setAlign(ALIGN_CENTER);
    text.setColor(textColor);

    outBox.setPosition(x, y);
    fillBox.setPosition(x + 2, y + 2);

    outBox.setFillColor(backgroundColor);
    outBox.setOutlineColor(outlineColor);
    outBox.setOutlineThickness(1);

    fillBox.setFillColor(fillColor);
}

void BoundedFloatSlider::render()
{
    update();
    draw();
}

void BoundedFloatSlider::update()
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

    auto widthFactor = value / (maxValue - minValue);

    std::string textString = GUIUtilities::floatToString(value, 3);

    fillBox.setSize(sf::Vector2f((width - 4) * widthFactor, height - (2 * 2)));
    text.setText(textString);
}

void BoundedFloatSlider::draw()
{
    window->draw(outBox);
    window->draw(fillBox);
    text.draw();
}

void BoundedFloatSlider::activeDraw()
{
    if (textBoxOpen)
    {
        inputTextBox.activeDraw();
    }
    else
    {
        auto mouse = sf::Mouse::getPosition(*this->window);
        int xDifference = mouse.x - clickX;

        if (xDifference != 0)
            mouseMoved = true;
        else
            mouseMoved = false;

        float factor = xDifference / width;

        value = std::min(std::max(clickValue + factor * (maxValue - minValue), minValue), maxValue);

        draw();
    }
    if (!this->active)
    {
        setActive();
    }
}

void BoundedFloatSlider::setActive()
{
    active = true;
    outBox.setFillColor(activeBackgroundColor);
    fillBox.setFillColor(activeFillColor);
}

void BoundedFloatSlider::setInactive()
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
    fillBox.setFillColor(fillColor);
    text.setColor(textColor);

}

void BoundedFloatSlider::setValue(float newValue)
{
    this->value = std::max(std::min(newValue, maxValue), minValue);
}
