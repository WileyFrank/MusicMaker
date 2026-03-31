#include "ToggleBox.h"
#include <algorithm>

ToggleBox::ToggleBox(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	sf::Color background,
	sf::Color outlineColor,
	sf::Color selectionColor,
	sf::Color backgroundHoverColor,
	sf::Color selectionHoverColor,
	std::function<void(bool)> stateChanged
)
	:RenderObject(0.0f, 0.0f, 0.0f, 0.0f),
	backgroundColor(background), outlineColor(outlineColor), selectionColor(selectionColor), backgroundHoverColor(backgroundHoverColor), selectionHoverColor(selectionHoverColor),
	outBox(sf::Vector2f(0.0f, 0.0f)), outlineWidth(2), selectionBox(sf::Vector2f(0.0f, 0.0f)),
	boxState(false),
	onStateChanged(std::move(stateChanged))
{
	this->type = GUIObject;
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);

	outBox.setFillColor(backgroundColor);
	outBox.setOutlineColor(outlineColor);
	outBox.setOutlineThickness((float)outlineWidth);
	
	selectionBox.setFillColor(backgroundColor);
	selectionBox.setOutlineThickness(0);

	applyIdleVisuals();
}

void ToggleBox::applyIdleVisuals()
{
	outBox.setFillColor(backgroundColor);
	selectionBox.setFillColor(boxState ? selectionColor : backgroundColor);
}

void ToggleBox::notifyStateChanged()
{
	if (onStateChanged)
	{
		onStateChanged(boxState);
	}
}

void ToggleBox::setChecked(bool checked, bool notifyCallback)
{
	if (boxState == checked)
	{
		return;
	}
	boxState = checked;
	applyIdleVisuals();
	if (notifyCallback)
	{
		notifyStateChanged();
	}
}

void ToggleBox::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	sf::FloatRect pixelRect = getResolvedRect();

	// Keep the control square: use the smaller edge and center in the allocated rect.
	const float side = std::min(pixelRect.width, pixelRect.height);
	const float offsetX = (pixelRect.width - side) * 0.5f;
	const float offsetY = (pixelRect.height - side) * 0.5f;
	setResolvedRect(sf::FloatRect(
		pixelRect.left + offsetX,
		pixelRect.top + offsetY,
		side,
		side));
	pixelRect = getResolvedRect();

	outBox.setPosition(sf::Vector2f(pixelRect.left, pixelRect.top));
	outBox.setSize(sf::Vector2f(pixelRect.width, pixelRect.height));

	const float inset = static_cast<float>(outlineWidth);
	const float innerWidth = std::max(0.0f, pixelRect.width - 2.0f * inset);
	const float innerHeight = std::max(0.0f, pixelRect.height - 2.0f * inset);
	selectionBox.setPosition(sf::Vector2f(pixelRect.left + inset, pixelRect.top + inset));
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
	applyIdleVisuals();
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
	applyIdleVisuals();
	notifyStateChanged();
}
