#include "Rectangle.h"

namespace {
void applyResolvedRectToShape(sf::RectangleShape& shape, const sf::FloatRect& pixelRect)
{
	const float outline = shape.getOutlineThickness();
	const float inset = outline > 0.0f ? outline : 0.0f;

	const float shapeWidth = std::max(0.0f, pixelRect.width - (2.0f * inset));
	const float shapeHeight = std::max(0.0f, pixelRect.height - (2.0f * inset));
	const float shapeX = pixelRect.left + inset;
	const float shapeY = pixelRect.top + inset;

	shape.setPosition(sf::Vector2f(shapeX, shapeY));
	shape.setSize(sf::Vector2f(shapeWidth, shapeHeight));
}
}

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

void Rectangle::setRectSpec(const RectSpec& spec)
{
	RenderObject::setRectSpec(spec);
}

void Rectangle::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	const sf::FloatRect pixelRect = getResolvedRect();
	/*std::cout
		<< "[Rectangle::resolveLayout] x=" << pixelRect.left
		<< " y=" << pixelRect.top
		<< " w=" << pixelRect.width
		<< " h=" << pixelRect.height
		<< std::endl;*/

	applyResolvedRectToShape(shape, pixelRect);
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
