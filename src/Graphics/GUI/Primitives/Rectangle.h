#pragma once
#include "SFML/Graphics.hpp"
#include "../RenderObject.h"
#include "../GUIUtilities.h"

class Rectangle : public RenderObject
{
	sf::RectangleShape shape;

public:
	Rectangle();
	Rectangle(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor = sf::Color(0, 1, 2), float outline = 0);

	void render() override;
	void update() override;
	void draw() override;

	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override{
		return std::make_pair(sf::Vector2f(shape.getPosition().x, shape.getPosition().y), sf::Vector2f(shape.getLocalBounds().width, shape.getLocalBounds().height));
	}
	RenderObject& getHoverObject() override;
};

