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

	void setRectSpec(const RectSpec& spec) override;
	void resolveLayout(const sf::FloatRect& parentRect) override;

	void render() override;
	void update() override;
	void draw() override;

	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override{
		const sf::FloatRect pixelRect = getResolvedRect();
		return std::make_pair(
			sf::Vector2f(pixelRect.left, pixelRect.top),
			sf::Vector2f(pixelRect.width, pixelRect.height)
		);
	}
	RenderObject& getHoverObject() override;
};

