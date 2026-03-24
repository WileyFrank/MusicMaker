#pragma once
#include "SFML/Graphics.hpp"
#include "../GUIUtilities.h"
#include "../RenderObject.h"

class PrimitiveRoundRectangle : public RenderObject
{
private:
	sf::ConvexShape shape;

	sf::Color color, outlineColor;
	float outlineThickness;
	float cornerRadius;
	int cornerPointCount;


public:
	PrimitiveRoundRectangle(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline, float cornerRadius, int cornerPointCount);
	void setRectSpec(const RectSpec& spec) override;
	void resolveLayout(const sf::FloatRect& parentRect) override;

	void render() override;
	void update() override;
	void draw() override;

	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {
		const sf::FloatRect pixelRect = getResolvedRect();
		return std::make_pair(
			sf::Vector2f(pixelRect.left, pixelRect.top),
			sf::Vector2f(pixelRect.width, pixelRect.height)
		);
	}
	RenderObject& getHoverObject() override;
	

};

