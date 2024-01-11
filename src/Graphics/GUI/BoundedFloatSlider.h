#pragma once
#include "TextBox.h"
#include "FloatSlider.h"
#include "Primitives/PrimitiveText.h"


class BoundedFloatSlider :
    public FloatSlider
{
private:


    sf::Color fillColor, activeFillColor;
    float minValue, maxValue;

    sf::RectangleShape fillBox;

public:
    //default will be between 0 and 1
    BoundedFloatSlider(float x, float y, float width, float height, sf::Color backgroundColor,
        sf::Color outlineColor, sf::Color fillColor, sf::Color textColor,
        sf::Color activeBackgroundColor, sf::Color activeFillColor, sf::Color activeTextColor);

    void setValue(float newValue);
    void setBounds(float min, float max)
    {
        this->minValue = min; this->maxValue = max;
    }
    
    void render() override;
    void update() override;
    void draw() override;
    void activeDraw() override;

    void setActive() override;
    void setInactive() override;

    void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        text.setWindow(window);
        this->inputTextBox.setWindow(window);
    }
};

//draw, setInactive