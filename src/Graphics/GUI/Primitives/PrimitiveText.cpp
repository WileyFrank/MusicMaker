#include "PrimitiveText.h"

#include "../../ResourceManager.h"

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
	:textString("Init"), fontString("resources/fonts/Century 751 Bold.otf"),
	font(*ResourceManager::getFont(fontString)), text(fontString, font), align(ALIGN_LEFT)
{
	type = GUIObject;
	this->x = 100;
	this->y = 100;
}

PrimitiveText::PrimitiveText(std::string text)
	:textString(text), fontString("resources/fonts/Century 751 Bold.otf"),
	font(*ResourceManager::getFont(fontString)), text(textString, font), align(ALIGN_LEFT)
{
	type = GUIObject;
	this->x = 100;
	this->y = 100;
}

PrimitiveText::PrimitiveText(float x, float y, int size, std::string text, std::string fontpath, Alignment align)
	:textString(text), fontString(fontpath), align(align),
	font(*ResourceManager::getFont(fontString)), text(textString, font, size)
{
	type = GUIObject;
	this->x = x;
	this->y = y;

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

PrimitiveText::PrimitiveText(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	int fontSize,
	std::string text,
	std::string fontpath,
	Alignment align)
	: PrimitiveText(0.0f, 0.0f, fontSize, std::move(text), std::move(fontpath), align)
{
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);
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

void PrimitiveText::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	const sf::FloatRect pixelRect = getResolvedRect();
	x = pixelRect.left;
	y = pixelRect.top;
	reloadText();
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

float PrimitiveText::getTextWidth(std::string widthText) {
	if (widthText.empty()) {
		widthText = textString;
	}

	float width = 0;
	sf::Uint32 previousChar = 0;

	for (size_t i = 0; i < widthText.size(); ++i) {
		sf::Uint32 currentChar = widthText[i];

		// Add kerning with the previous character
		if (i > 0) {
			width += text.getFont()->getKerning(previousChar, currentChar, text.getCharacterSize());
		}

		// Add the advance of the current character
		width += text.getFont()->getGlyph(currentChar, text.getCharacterSize(), false).advance;

		previousChar = currentChar;
	}

	return width;
}
