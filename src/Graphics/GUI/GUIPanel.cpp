#include "GUIPanel.h"

GUIPanel::GUIPanel()
	:x(0), y(0), width(100), height(100), 
	backgroundColor(sf::Color(255,255,255)), outlineColor(sf::Color(255, 255, 255))
{
	type = GUIObject;
	auto rect = new Rectangle(x, y, width, height, backgroundColor, outlineColor);

	elements.push_back(rect);
}

GUIPanel::GUIPanel(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline)
	:x(x), y(y), width(width), height(height), currentX(x + 5), currentY(y),
	backgroundColor(color), outlineColor(outlineColor)
{
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
