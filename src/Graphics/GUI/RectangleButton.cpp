#include <algorithm>
#include <iostream>

#include "SFML/Graphics.hpp"

#include "RectangleButton.h"

RectangleButton::RectangleButton(float xIn, float yIn, float width, float height, sf::Color colorIn)
{

	positionX = xIn;
	positionY = yIn;
	this->width = width;
	this->height = height;
	this->baseColor = colorIn;
	this->hoverColor = sf::Color(
		std::min(colorIn.r + 50,255), 
		std::min(colorIn.g + 50, 255),
		std::min(colorIn.b + 50, 255), 255);
	this->color = colorIn;

	this->window = nullptr;
	shape = new sf::RectangleShape(sf::Vector2f(width, height));

	shape->setFillColor(colorIn);
	shape->setPosition(positionX, positionY);

	font = ResourceManager::getFont("resources/fonts/Dream Orphans Bd.otf");
	if (font == nullptr) {
		std::cerr << "Failed to load font file\n";
		return;
	}


	text.setFont(*font);        // Set the font to the text
	text.setString("Button");  // Set the string to display

	float i = 2;
	text.setCharacterSize((int)(height / i)); // Set the character size in pixels

	while (text.getLocalBounds().width > width)
	{
		i += 0.25;
		text.setCharacterSize((int)(height / i)); // Set the character size in pixels

	}


	text.setFillColor(sf::Color::White); // Set the text color
	text.setPosition(positionX + width / 2.0f - text.getLocalBounds().width / 2.0f, positionY + height / 2.0f - text.getLocalBounds().height / 2.0f);


}

RectangleButton::~RectangleButton()
{
	delete shape;
}

void RectangleButton::setText(std::string textString)
{
	text.setString(textString);  // Set the string to display
	text.setPosition(positionX + width / 2.0f - text.getLocalBounds().width / 2.0f, positionY + height / 2.0f - text.getLocalBounds().height / 2.0f);
}

void RectangleButton::update()
{
	if (window != nullptr)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);

		if (mousePosition.x > this->positionX && mousePosition.x < this->positionX + this->width && 
			mousePosition.y > this->positionY && mousePosition.y < this->positionY + this->height)
		{
			this->color = hoverColor;
			shape->setFillColor(hoverColor);
			this->hover = true;
		}
		else
		{
			shape->setFillColor(baseColor);
		}
	}
	else
	{
		shape->setFillColor(baseColor);
	}
}
	

void RectangleButton::draw()
{
	window->draw(*this->shape);
	window->draw(this->text);
	return;
}

void RectangleButton::render()
{

	if (window != nullptr)
	{
		update();
		draw();
		return;
	}
	else
	{
		std::cerr << "Error: Attempted to draw Rectangle Button when window is uninitialized\n";
	}
}

RenderObject& RectangleButton::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	auto hoverArea = this->getHoverArea();

	if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}
