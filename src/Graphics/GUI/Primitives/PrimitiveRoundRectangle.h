#pragma once
#include "SFML/Graphics.hpp"
#include "../GUIUtilities.h"
#include "../RenderObject.h"

class PrimitiveRoundRectangle : public RenderObject
{
private:
	float x, y, width, height;

	sf::ConvexShape shape;

	sf::Color color, outlineColor;


public:
	PrimitiveRoundRectangle(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline, float cornerRadius, int cornerPointCount);

	void render() override;
	void update() override;
	void draw() override;

	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {
		return std::make_pair(sf::Vector2f(shape.getPosition().x, shape.getPosition().y), sf::Vector2f(shape.getLocalBounds().width, shape.getLocalBounds().height));
	}
	RenderObject& getHoverObject() override;
	

};

