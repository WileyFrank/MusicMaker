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

    TextBox<float> inputTextBox;

public:
    //default will be between 0 and 1
    BoundedFloatSlider(float x, float y, float width, float height, sf::Color backgroundColor,
        sf::Color outlineColor, sf::Color fillColor, sf::Color textColor,
        sf::Color activeBackgroundColor, sf::Color activeFillColor, sf::Color activeTextColor);

    void setValue(float newValue);
    
    void render() override;
    void update() override;
    void draw() override;
    void activeDraw() override;

    void setActive() override;
    void setInactive() override;

    //unchanged


    //void hoverDraw() override;
    //void setHoverstate() override;
    //void setUnhover() override;

    /*void keyboardInput(sf::Uint32 input) override
    {
        if (textBoxOpen)
        {
            inputTextBox.keyboardInput(input);
        }
    }*/
    /*void arrowKeyInput(sf::Keyboard::Key key) override
    {
        if (textBoxOpen)
        {
            inputTextBox.arrowKeyInput(key);
        }
    }*/

    /*void onClick() override;*/

    /*virtual std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        return std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, height));
    }*/
    /*RenderObject& getHoverObject()
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        auto hoverArea = this->getHoverArea();

        if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
        {
            return *this;
        }

        return *GUIUtilities::getEmptyRenderObject();
    }*/
    /*RenderObject& getActiveObject() override
    {
        if (textBoxOpen)
        {
            inputTextBox.getActiveObject();
        }
        return *this;
    }*/

    /*void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        text.setWindow(window);
        inputTextBox.setWindow(window);
    }*/
};

//draw, setInactive