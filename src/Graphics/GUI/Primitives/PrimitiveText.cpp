#include "PrimitiveText.h"

void PrimitiveText::reloadText()
{
	text.setString(textString);

	switch (align)
	{
	case ALIGN_LEFT:
		this->text.setOrigin(sf::Vector2f(0.0f, 0.0f));
		break;
	case ALIGN_CENTER:
		this->text.setOrigin(sf::Vector2f(this->text.getLocalBounds().width / 2, 0.0f));
		break;
	case ALIGN_RIGHT:
		this->text.setOrigin(sf::Vector2f(this->text.getLocalBounds().width, 0.0f));
		break;
	}

	this->text.setPosition(x, y);
}

PrimitiveText::PrimitiveText()
	:x(100),y(100),
	textString("Init"), fontString("resources/fonts/Century 751 Bold.otf"),
	font(*ResourceManager::getFont(fontString)), text(fontString, font)
{

}

PrimitiveText::PrimitiveText(std::string text)
	:x(100), y(100),
	textString(text), fontString("resources/fonts/Century 751 Bold.otf"),
	font(*ResourceManager::getFont(fontString)), text(textString, font)
{
}

PrimitiveText::PrimitiveText(float x, float y, int size, std::string text, std::string fontpath, Alignment align)
	:x(x), y(y),
	textString(text), fontString(fontpath), align(align),
	font(*ResourceManager::getFont(fontString)), text(textString, font, size)
{
	switch (align)
	{
	case ALIGN_LEFT:
		this->text.setOrigin(sf::Vector2f(0.0f, 0.0f));
		break;
	case ALIGN_CENTER:
		this->text.setOrigin(sf::Vector2f(this->text.getLocalBounds().width / 2, 0.0f));
		break;
	case ALIGN_RIGHT:
		this->text.setOrigin(sf::Vector2f(this->text.getLocalBounds().width, 0.0f) );
		break;
	}

	this->text.setPosition(x, y);
}

void PrimitiveText::render()
{
	update();
	draw();
}

void PrimitiveText::update()
{
	
}

void PrimitiveText::draw()
{
	window->draw(text);
}

RenderObject& PrimitiveText::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	auto hoverArea = this->getHoverArea();

	if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}
