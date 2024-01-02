#pragma once
#include "GUIUtilities.h"
#include "RenderObject.h"
#include "Primitives/PrimitiveText.h"


template <typename T>
class TextBox :
    public RenderObject
{
private:

    T value; //The value of the string as type T

    float x, y, width, height;
    int fontSize, textDisplayStart = 0;

    std::string textString;

    sf::RectangleShape box;
    PrimitiveText text;

    void initializeTextBox()
    {
        box = sf::RectangleShape(sf::Vector2f(width, height));
        box.setPosition(sf::Vector2f(x, y));
        box.setFillColor(sf::Color(255, 255, 255));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(0, 0, 255));

    }

public:
    TextBox(float x, float y, float width, float height, int size, std::string textString = "0") 
        :x(x), y(y), width(width), height(height), fontSize(size), textString(textString)
    {
        initializeTextBox();
        this->type = GUIObject;
    }
    
    bool ValidateInput()
    {
        // Check if the input string can be converted to type T
        T value;
        std::istringstream iss(textString);
        iss >> value;
        return !(iss.fail() || !iss.eof());
    }

    void render() override
    {
        update();
        draw();
    }
    void update() override {}
    void draw() override
    {
        window->draw(box);
    }
    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {
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
};

