#include "GUIButton.h"

void GUIButton::setActive()
{
	active = true;
	color = activeFillColor;
	if (toggleMode)
	{
		togglePressed = true;
	}
}

void GUIButton::setInactive()
{
	active = false;
	if (!toggleMode)
	{
		togglePressed = false;
		color = baseColor;
		applyNormalVisuals();
	}
}

void GUIButton::setUnselected()
{
	active = false;
	togglePressed = false;
	color = baseColor;
	applyNormalVisuals();
}

void GUIButton::onClick()
{
	setActive();
	if (onClickAction)
	{
		onClickAction();
	}
}

void GUIButton::onMouseButtonReleased(sf::Mouse::Button button)
{
	if (button == sf::Mouse::Left && !toggleMode)
	{
		setInactive();
	}
}
