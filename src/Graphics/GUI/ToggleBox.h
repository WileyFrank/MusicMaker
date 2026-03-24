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
    ToggleBox(
        const RectSpec& rectSpec,
        const MarginSpec& marginSpec,
        sf::Color background,
        sf::Color outlineColor,
        sf::Color selectionColor,
        sf::Color backgroundHoverColor,
        sf::Color selectionHoverColor
    );
        
    bool getState() { return boxState; }

    void resolveLayout(const sf::FloatRect& parentRect) override;
    void update() override;
    void draw() override;
    void hoverDraw() override;
    void activeDraw() override;

    void setHoverstate();
    void setUnhover();

    void setActive() override;
    void setInactive() override;

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        const sf::FloatRect pixelRect = getResolvedRect();
        return std::make_pair(
            sf::Vector2f(pixelRect.left, pixelRect.top),
            sf::Vector2f(pixelRect.width, pixelRect.height)
        );
    }
    RenderObject& getHoverObject();

    virtual void onClick();


};

