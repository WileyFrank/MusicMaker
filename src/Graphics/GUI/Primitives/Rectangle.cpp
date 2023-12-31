#include "Rectangle.h"

Rectangle::Rectangle()
	:shape(sf::Vector2f(100, 100))
{
	type = PrimitiveRenderObject;
	shape.setPosition(sf::Vector2f(0, 0));
	shape.setFillColor(sf::Color(255,255,255));
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color(255, 0, 0));
}

Rectangle::Rectangle(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline)
	:shape(sf::Vector2f(width, height))
{
	type = PrimitiveRenderObject;
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(color);

	if (outline > 0.0f)
	{
		shape.setOutlineThickness(outline);
		shape.setOutlineColor(outlineColor);
	}
}

void Rectangle::render()
{
	update();
	draw();
}

void Rectangle::update()
{
}

void Rectangle::draw()
{
	window->draw(shape);
}

RenderObject& Rectangle::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	auto hoverArea = this->getHoverArea();

	if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}
