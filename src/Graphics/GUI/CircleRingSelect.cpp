#include "CircleRingSelect.h"
#include "GUIUtilities.h"

void CircleRingSelect::positionOnCircle()
{
	auto mouse = sf::Mouse::getPosition(*window);

	float deltaX = mouse.x - x;
	float deltaY = y - mouse.y;

	if (deltaX == 0)
		deltaX = 0.01f;
	if (deltaY == 0)
		deltaY = 0.01f;

	float theta = std::atan(deltaY / deltaX);

	//This adjusts atan to be used 360 deg
	if (deltaX < 0)
		theta += 3.14159f;

	if (theta < 0)
		theta += 3.14159f * 2;

	angleRadians = theta;

	if (angleSlider != nullptr)
	{
		float angleDegrees = theta * 180 / 3.14159f;
		angleSlider->setValue(angleDegrees);
	}

	updateAngle();
}

void CircleRingSelect::updateAngle()
{

	this->selectionX = x + std::cos(angleRadians) * ringRadius;
	this->selectionY = y - std::sin(angleRadians) * ringRadius;

	selectionCircle.setPosition(sf::Vector2f(selectionX, selectionY));
}

CircleRingSelect::CircleRingSelect(float x, float y, float ringRadius, float circleRadius, sf::Color ringColor, sf::Color circleColor, sf::Color ringOutlineColor, sf::Color circleOutlineColor, sf::Color hoverCircleColor, sf::Color activeCircleColor)
	:ringRadius(ringRadius), circleRadius(circleRadius),
	ringColor(ringColor), circleColor(circleColor), ringOutlineColor(ringOutlineColor), 
	circleOutlineColor(circleOutlineColor), activeCircleColor(activeCircleColor), hoverCircleColor(hoverCircleColor),
	selectionRing(ringRadius, (int)ringRadius), selectionCircle(circleRadius, (int)circleRadius * 2)
{
	this->type = GUIObject;

	this->x = x;
	this->y = y;

	this->angleRadians = 0.0f;
	this->inDegrees = true;

	this->selectionX = x + ringRadius;
	this->selectionY = y;

	selectionRing.setPosition(sf::Vector2f(x, y));
	selectionCircle.setPosition(sf::Vector2f(x + ringRadius, y));

	selectionRing.setFillColor(ringColor);
	selectionCircle.setFillColor(circleColor);

	selectionRing.setOutlineColor(ringOutlineColor);
	selectionCircle.setOutlineColor(circleOutlineColor);

	selectionRing.setOutlineThickness(2);
	selectionCircle.setOutlineThickness(1);

	selectionRing.setOrigin(sf::Vector2f(ringRadius, ringRadius));
	selectionCircle.setOrigin(sf::Vector2f(circleRadius, circleRadius));
}

void CircleRingSelect::render()
{
	update();
	draw();
}

void CircleRingSelect::update()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		active = false;
		setInactive();
	}
	else if (active)
	{
		positionOnCircle();
	}
	else if (angleSlider != nullptr)
	{
		angleRadians = angleSlider->getValue() * 3.14159f / 180;
		updateAngle();
	}

}

void CircleRingSelect::draw()
{
	window->draw(selectionRing);
	window->draw(selectionCircle);
}

void CircleRingSelect::hoverDraw()
{
	setHoverstate();
	draw();
}

void CircleRingSelect::activeDraw()
{
	setActive();
	draw();
}

void CircleRingSelect::setHoverstate()
{
	selectionCircle.setFillColor(hoverCircleColor);
}

void CircleRingSelect::setUnhover()
{
	selectionCircle.setFillColor(circleColor);
}

void CircleRingSelect::setActive()
{
	this->active = true;
	selectionCircle.setOutlineColor(activeCircleColor);
}

void CircleRingSelect::setInactive()
{
	this->active = false;
	selectionCircle.setOutlineColor(circleOutlineColor);
}

RenderObject& CircleRingSelect::getHoverObject()
{
	auto mouse = sf::Mouse::getPosition(*window);

	float deltaX = mouse.x - this->selectionX;
	float deltaY = mouse.y - this->selectionY;

	double distanceSquared = std::pow(deltaX, 2) + std::pow(deltaY, 2);

	if (distanceSquared < std::pow(circleRadius, 2))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}
