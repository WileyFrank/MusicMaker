#pragma once
#include "RenderObject.h"
#include "GUIUtilities.h"


class ToggleBox :
    public RenderObject
{
private:
    bool boxState;
    int outlineWidth;

    sf::RectangleShape outBox, selectionBox;
    sf::Color backgroundColor, outlineColor, selectionColor, backgroundHoverColor, selectionHoverColor;

public:
    ToggleBox(float x, float y, float width, float height, sf::Color background, sf::Color outlineColor, sf::Color selectionColor, sf::Color backgroundHoverColor, sf::Color selectionHoverColor);
        
    bool getState() { return boxState; }

    void update() override;
    void draw() override;
    void hoverDraw() override;
    void activeDraw() override;

    void setHoverstate();
    void setUnhover();

    void setActive() override;
    void setInactive() override;

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        return std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, height));
    }
    RenderObject& getHoverObject();

    virtual void onClick();


};

