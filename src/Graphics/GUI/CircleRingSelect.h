#pragma once
#include "RenderObject.h"
#include "BoundedFloatSlider.h"

class CircleRingSelect :
    public RenderObject
{
private:
    float angleRadians, ringRadius, circleRadius, selectionX, selectionY;

    //true = deg, false = rad
    bool inDegrees;

    sf::CircleShape selectionRing, selectionCircle;

    sf::Color ringColor, circleColor, ringOutlineColor, circleOutlineColor, hoverCircleColor, activeCircleColor;

    BoundedFloatSlider* angleSlider = nullptr;

    void positionOnCircle();
    void updateAngle();

public:
    CircleRingSelect(float x, float y, float ringRadius, float circleRadius, sf::Color ringColor, sf::Color circleColor, sf::Color ringOutlineColor, sf::Color circleOutlineColor, sf::Color hoverCircleColor, sf::Color activeCircleColor);

    float getAngle() { return angleRadians; }

    void render() override;
    void update() override;
    void draw() override;
    void hoverDraw() override;
    void activeDraw() override;

    void setHoverstate();
    void setUnhover();

    void setActive() override;
    void setInactive() override;

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        return std::make_pair(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
    }
    RenderObject& getHoverObject();

    void setSlider(BoundedFloatSlider* slider)
    {
        this->angleSlider = slider;
    }

};

