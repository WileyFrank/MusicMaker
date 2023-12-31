#pragma once
#include "RenderObject.h"

class NullRenderObject : public RenderObject
{
public:
    NullRenderObject() { this->type = EmptyRenderObject; }
    NullRenderObject(RenderObjectType type) { this->type = type; }

    void draw() override { return; }
    void update() override { return; }
    void render() override { return; }
    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea()
    {
        return std::make_pair(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
    }
    RenderObject& getHoverObject() override { return *this; }

};