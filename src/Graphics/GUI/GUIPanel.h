#pragma once
#include "RenderObject.h"
#include "Primitives/Rectangle.h"
#include "Primitives/PrimitiveText.h"
#include "Primitives/PrimitiveRoundRectangle.h"

class GUIPanel :
    public RenderObject
{
private: 
    float currentX, currentY;
    float padding = 5.0f;

    std::vector<RenderObject*> elements;

    sf::Color backgroundColor, outlineColor;

public:
    GUIPanel();
    GUIPanel(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor = sf::Color(0, 1, 2), float outline = 0);

    void addText(std::string text, int size, sf::Color color);


    void render() override;
    void update() override;
    void draw() override;

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        return std::make_pair(sf::Vector2f(x,y), sf::Vector2f(width, height));
    }
    RenderObject& getHoverObject();

    void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        for (auto& element : elements)
        {
            element->setWindow(window);
        }
    }

};

