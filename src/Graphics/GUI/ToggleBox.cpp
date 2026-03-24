#include "ToggleBox.h"
#include <algorithm>

ToggleBox::ToggleBox(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	sf::Color background,
	sf::Color outlineColor,
	sf::Color selectionColor,
	sf::Color backgroundHoverColor,
	sf::Color selectionHoverColor
)
	:RenderObject(0.0f, 0.0f, 0.0f, 0.0f),
	backgroundColor(background), outlineColor(outlineColor), selectionColor(selectionColor), backgroundHoverColor(backgroundHoverColor), selectionHoverColor(selectionHoverColor),
	outBox(sf::Vector2f(0.0f, 0.0f)), outlineWidth(2), selectionBox(sf::Vector2f(0.0f, 0.0f)),
	boxState(false)
{
	this->type = GUIObject;
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);

	outBox.setFillColor(backgroundColor);
	outBox.setOutlineColor(outlineColor);
	outBox.setOutlineThickness(outlineWidth);
	
	selectionBox.setFillColor(backgroundColor);
	selectionBox.setOutlineThickness(0);
}

void ToggleBox::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	const sf::FloatRect pixelRect = getResolvedRect();

	outBox.setPosition(sf::Vector2f(pixelRect.left, pixelRect.top));
	outBox.setSize(sf::Vector2f(pixelRect.width, pixelRect.height));

	const float innerWidth = std::max(0.0f, pixelRect.width - (4.0f * outlineWidth));
	const float innerHeight = std::max(0.0f, pixelRect.height - (4.0f * outlineWidth));
	selectionBox.setPosition(sf::Vector2f(pixelRect.left + (2.0f * outlineWidth), pixelRect.top + (2.0f * outlineWidth)));
	selectionBox.setSize(sf::Vector2f(innerWidth, innerHeight));
}

void ToggleBox::update()
{
}

void ToggleBox::draw()
{
	window->draw(outBox);
	if (boxState)
	{
		window->draw(selectionBox);
	}
}

void ToggleBox::hoverDraw()
{
	setHoverstate();
	draw();
}

void ToggleBox::activeDraw()
{
}

void ToggleBox::setHoverstate()
{
	selectionBox.setFillColor(selectionHoverColor);
	outBox.setFillColor(backgroundHoverColor);
}

void ToggleBox::setUnhover()
{
	selectionBox.setFillColor(selectionColor);
	outBox.setFillColor(backgroundColor);
}

void ToggleBox::setActive()
{
}

void ToggleBox::setInactive()
{
}

RenderObject& ToggleBox::getHoverObject()
{
	auto mouse = sf::Mouse::getPosition(*window);

	if (GUIUtilities::positionInBounds(mouse, getHoverArea()))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}

void ToggleBox::onClick()
{
	boxState = !boxState;
}
