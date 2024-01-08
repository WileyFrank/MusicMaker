#pragma once
#include "RenderObject.h"
#include "Primitives/PrimitiveText.h"


class FloatSlider :
    public RenderObject
{
private:
    sf::Color backgroundColor, outlineColor, fillColor, textColor, activeFillColor, activeBackgroundColor;
    float minValue, maxValue, value, clickValue;

    int clickX;

    sf::RectangleShape outBox, fillBox;
    PrimitiveText text;

public:
    //default will be between 0 and 1
    FloatSlider(float x, float y, float width, float height, sf::Color backgroundColor, 
        sf::Color outlineColor, sf::Color fillColor, sf::Color textColor, 
        sf::Color activeBackgroundColor, sf::Color activeFillColor)
        :backgroundColor(backgroundColor), outlineColor(outlineColor), fillColor(fillColor),
        textColor(textColor), activeFillColor(activeFillColor), activeBackgroundColor(activeBackgroundColor),
        outBox(sf::Vector2f(width, height)), fillBox(sf::Vector2f(0.0f, height - (2 * 2))),
        text(x + width / 2, y, (int)(height * .75f), "0.00")
    {
        this->type = GUIObject;

        this->x = x;
        this->y = y;
        this-> width = width;
        this->height = height;

        this->minValue = 0.0f;
        this->maxValue = 1.0f;
        this->value = 0.50f;
        text.setAlign(ALIGN_CENTER);
        text.setColor(textColor);

        outBox.setPosition(x, y);
        fillBox.setPosition(x + 2, y + 2);

        outBox.setFillColor(backgroundColor);
        outBox.setOutlineColor(outlineColor);
        outBox.setOutlineThickness(1);

        fillBox.setFillColor(fillColor);
    }

    void render() override
    {
        update();
        draw();
    }
    void update() override
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
    void draw() override
    {
        window->draw(outBox);
        window->draw(fillBox);
        text.draw();
    }
    void hoverDraw() override
    {
        //outBox.setOutlineColor(fillColor);
        outBox.setFillColor(activeBackgroundColor);
        draw();
    }
    void activeDraw()
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
    };

    void setActive() override
    {
        active = true;
        outBox.setFillColor(activeBackgroundColor);
        fillBox.setFillColor(activeFillColor);
    }
    void setInactive() override
    {
        active = false;
        outBox.setFillColor(backgroundColor);
        fillBox.setFillColor(fillColor);
        text.setColor(textColor);

    }

    void setHoverstate() override
    {
        outBox.setFillColor(activeBackgroundColor);
    }
    void setUnhover() override
    {
        outBox.setFillColor(backgroundColor);

    }

    void onClick() override
    {
        auto mouse = sf::Mouse::getPosition(*window);
        clickX = mouse.x;
        clickValue = value;
        setActive();
    }

    virtual std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        return std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, height));
    }
    RenderObject& getHoverObject()
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        auto hoverArea = this->getHoverArea();

        if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
        {
            return *this;
        }

        return *GUIUtilities::getEmptyRenderObject();
    }

    void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        text.setWindow(window);
    }
};

