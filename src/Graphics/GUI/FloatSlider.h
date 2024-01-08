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
        sf::Color activeBackgroundColor, sf::Color activeFillColor);

    void render() override;
    void update() override;
    void draw() override;
    void hoverDraw() override;
    void activeDraw() override;

    void setActive() override;
    void setInactive() override;

    void setHoverstate() override;
    void setUnhover() override;

    void onClick() override;

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