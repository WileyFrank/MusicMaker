#include "GUIPanel.h"

GUIPanel::GUIPanel()
	:backgroundColor(sf::Color(255,255,255)), outlineColor(sf::Color(255, 255, 255))
{
	type = GUIObject;

	this->x = 0;
	this->y = 0;
	this->width = 100;
	this->height = 100;

	auto rect = new Rectangle(x, y, width, height, backgroundColor, outlineColor);

	elements.push_back(rect);
}

GUIPanel::GUIPanel(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline)
	:currentX(x + 5), currentY(y),
	backgroundColor(color), outlineColor(outlineColor)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	PrimitiveRoundRectangle* panel = new PrimitiveRoundRectangle(x, y, width, height, color, outlineColor, outline, 3,10);

	elements.push_back(panel);
}

void GUIPanel::addText(std::string text, int size, sf::Color color)
{
	PrimitiveText* textObject = new PrimitiveText(currentX, currentY, size, text, "resources/fonts/Sansation_Regular.ttf", ALIGN_LEFT);

	currentY += textObject->getHoverArea().second.y + padding;
	textObject->setColor(color);
	textObject->setWindow(window);

	elements.push_back(textObject);
}

void GUIPanel::render()
{
	for (auto& element : elements)
	{
		element->render();
	}
}

void GUIPanel::update()
{
	for (auto& element : elements)
	{
		element->update();
	}
}

void GUIPanel::draw()
{
	for (auto& element : elements)
	{
		element->draw();
	}
}

RenderObject& GUIPanel::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	auto hoverArea = this->getHoverArea();

	if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}
