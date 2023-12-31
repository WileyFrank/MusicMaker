#include "SheetMusicElement.h"

GUIPanel* SheetMusicElement::addPanel(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outlineWidth)
{
	GUIPanel* panel = new GUIPanel(x, y, width, height, color, outlineColor, outlineWidth);
	panel->setWindow(window);
	objects.push_back(std::move(panel));

	return panel;
}

void SheetMusicElement::draw()
{
}

void SheetMusicElement::render()
{
	this->update();
	this->draw();
}
