#include "ToggleBox.h"

ToggleBox::ToggleBox(float x, float y, float width, float height, sf::Color background, sf::Color outlineColor, sf::Color selectionColor, sf::Color backgroundHoverColor, sf::Color selectionHoverColor)
	:RenderObject(x, y, width, height),
	backgroundColor(backgroundColor), outlineColor(outlineColor), selectionColor(selectionColor), backgroundHoverColor(backgroundHoverColor), selectionHoverColor(selectionHoverColor),
	outBox(sf::Vector2f(width, height)), outlineWidth(2), selectionBox(sf::Vector2f(width - (4 * outlineWidth), height - (4 * outlineWidth))),
	boxState(false)
{
	this->type = GUIObject;

	outBox.setFillColor(backgroundColor);
	outBox.setOutlineColor(outlineColor);
	outBox.setOutlineThickness(outlineWidth);
	
	selectionBox.setFillColor(backgroundColor);
	selectionBox.setOutlineThickness(0);

	outBox.setPosition(sf::Vector2f(x, y));
	selectionBox.setPosition(sf::Vector2f(x + (2 * outlineWidth), y + (2 * outlineWidth)));

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
