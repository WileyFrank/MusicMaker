#pragma once
#include "Container.h"
#include "Primitives/Rectangle.h"
#include "Primitives/PrimitiveText.h"
#include "Primitives/PrimitiveRoundRectangle.h"

class GUIPanel :
    public Container
{
private: 
    float currentX, currentY;
    float padding = 5.0f;
    RenderObject* backgroundShape = nullptr;

    sf::Color backgroundColor, outlineColor;

public:
    GUIPanel();
    GUIPanel(
        float x,
        float y,
        float width,
        float height,
        sf::Color color,
        sf::Color outlineColor = sf::Color(0, 1, 2),
        float outline = 0,
        float cornerSize = 0
    );

    GUIPanel(
        const RectSpec& rectSpec,
        sf::Color color,
        sf::Color outlineColor = sf::Color(0, 1, 2),
        float outline = 0,
        float cornerSize = 0,
        MarginSpec marginSpec = { Px(0), Px(0), Px(0), Px(0) }
    );

    void setRectSpec(const RectSpec& spec) override;
    void addText(std::string text, int size, sf::Color color);
    void resolveChildrenLayout() override;
    void resolveLayout(const sf::FloatRect& parentRect) override;

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        const sf::FloatRect pixelRect = getPixelRect();
        return std::make_pair(
            sf::Vector2f(pixelRect.left, pixelRect.top),
            sf::Vector2f(pixelRect.width, pixelRect.height)
        );
    }
    RenderObject& getHoverObject();
};

