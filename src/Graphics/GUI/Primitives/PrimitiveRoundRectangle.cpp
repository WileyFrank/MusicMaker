#include "PrimitiveRoundRectangle.h"
#include <algorithm>

PrimitiveRoundRectangle::PrimitiveRoundRectangle(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline, float cornerRadius, int cornerPointCount)
    : shape(4 * cornerPointCount + 8),
    color(color), outlineColor(outlineColor),
    outlineThickness(outline), cornerRadius(cornerRadius), cornerPointCount(cornerPointCount)
{
    setResolvedRect(sf::FloatRect(x, y, width, height));
    resolveLayout(sf::FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
}

void PrimitiveRoundRectangle::setRectSpec(const RectSpec& spec)
{
    RenderObject::setRectSpec(spec);
}

void PrimitiveRoundRectangle::resolveLayout(const sf::FloatRect& parentRect)
{
    RenderObject::resolveLayout(parentRect);
    const sf::FloatRect pixelRect = getResolvedRect();

    const float inset = outlineThickness > 0.0f ? outlineThickness : 0.0f;
    const float shapeWidth = std::max(0.0f, pixelRect.width - (2.0f * inset));
    const float shapeHeight = std::max(0.0f, pixelRect.height - (2.0f * inset));
    const float shapeX = pixelRect.left + inset;
    const float shapeY = pixelRect.top + inset;

    const float maxCornerRadius = std::max(0.0f, std::min(shapeWidth, shapeHeight) * 0.5f);
    const float appliedCornerRadius = std::min(cornerRadius, maxCornerRadius);

    shape = GUIUtilities::createRoundedRectangle(shapeWidth, shapeHeight, appliedCornerRadius, cornerPointCount);
    shape.setFillColor(color);
    shape.setOutlineColor(outlineColor);
    shape.setOutlineThickness(outlineThickness);
    shape.setPosition(shapeX, shapeY);
}

void PrimitiveRoundRectangle::render()
{
    update();
    draw();
}

void PrimitiveRoundRectangle::update()
{
}

void PrimitiveRoundRectangle::draw()
{
    if (window != nullptr)
    {
        window->draw(shape);
    }
}

RenderObject& PrimitiveRoundRectangle::getHoverObject()
{
	auto mouse = sf::Mouse::getPosition(*window);

	if (GUIUtilities::positionInBounds(mouse, this->getHoverArea()))
	{
		return *this;
	}
    return *GUIUtilities::getEmptyRenderObject();
}
