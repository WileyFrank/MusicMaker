#include "BoundedFloatSlider.h"

BoundedFloatSlider::BoundedFloatSlider(float x, float y, float width, float height, sf::Color backgroundColor, sf::Color outlineColor, sf::Color fillColor, sf::Color textColor, sf::Color activeBackgroundColor, sf::Color activeFillColor)
    :backgroundColor(backgroundColor), outlineColor(outlineColor), fillColor(fillColor),
    textColor(textColor), activeFillColor(activeFillColor), activeBackgroundColor(activeBackgroundColor),
    outBox(sf::Vector2f(width, height)), fillBox(sf::Vector2f(0.0f, height - (2 * 2))),
    text(x + width / 2, y, (int)(height * .75f), "0.00")
{
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
    if (active)
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            setInactive();
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

void BoundedFloatSlider::hoverDraw()
{
    //outBox.setOutlineColor(fillColor);
    outBox.setFillColor(activeBackgroundColor);
    draw();
}

void BoundedFloatSlider::activeDraw()
{
    auto mouse = sf::Mouse::getPosition(*window);
    int xDifference = mouse.x - clickX;

    float factor = xDifference / width;

    value = std::min(std::max(clickValue + factor * (maxValue - minValue), minValue), maxValue);

    if (!this->active)
    {
        setActive();
    }
    draw();
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
    outBox.setFillColor(backgroundColor);
    fillBox.setFillColor(fillColor);
    text.setColor(textColor);

}

void BoundedFloatSlider::setHoverstate()
{
    outBox.setFillColor(activeBackgroundColor);
}

void BoundedFloatSlider::setUnhover()
{
    outBox.setFillColor(backgroundColor);

}

void BoundedFloatSlider::onClick()
{
    auto mouse = sf::Mouse::getPosition(*window);
    clickX = mouse.x;
    clickValue = value;
    setActive();
}
