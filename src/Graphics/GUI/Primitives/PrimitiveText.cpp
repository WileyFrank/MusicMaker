#include "PrimitiveText.h"

#include <algorithm>
#include <cmath>

#include "../../ResourceManager.h"
#include "../UnitValueResolver.h"

void PrimitiveText::reloadText()
{
	text.setString(textString);
	const sf::FloatRect b = text.getLocalBounds();

	if (!layoutResolved)
	{
		switch (align)
		{
		case ALIGN_LEFT:
			text.setOrigin(sf::Vector2f(0.0f, 0.0f));
			break;
		case ALIGN_CENTER:
			text.setOrigin(sf::Vector2f(b.width / 2.0f, 0.0f));
			break;
		case ALIGN_RIGHT:
			text.setOrigin(sf::Vector2f(b.width, 0.0f));
			break;
		}
		text.setPosition(x, y);
		return;
	}

	float ox = b.left;
	switch (align)
	{
	case ALIGN_LEFT:
		ox = b.left;
		break;
	case ALIGN_CENTER:
		ox = b.left + b.width * 0.5f;
		break;
	case ALIGN_RIGHT:
		ox = b.left + b.width;
		break;
	}

	float oy = b.top;
	switch (verticalAlign)
	{
	case VERTICAL_ALIGN_TOP:
		oy = b.top;
		break;
	case VERTICAL_ALIGN_MIDDLE:
		oy = b.top + b.height * 0.5f;
		break;
	case VERTICAL_ALIGN_BOTTOM:
		oy = b.top + b.height;
		break;
	}

	text.setOrigin(ox, oy);

	float px = layoutCell.left;
	switch (align)
	{
	case ALIGN_LEFT:
		px = layoutCell.left;
		break;
	case ALIGN_CENTER:
		px = layoutCell.left + layoutCell.width * 0.5f;
		break;
	case ALIGN_RIGHT:
		px = layoutCell.left + layoutCell.width;
		break;
	}

	float py = layoutCell.top;
	switch (verticalAlign)
	{
	case VERTICAL_ALIGN_TOP:
		py = layoutCell.top;
		break;
	case VERTICAL_ALIGN_MIDDLE:
		py = layoutCell.top + layoutCell.height * 0.5f;
		break;
	case VERTICAL_ALIGN_BOTTOM:
		py = layoutCell.top + layoutCell.height;
		break;
	}

	text.setPosition(px, py);
	x = px;
	y = py;
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

PrimitiveText::PrimitiveText(float x, float y, int size, std::string text, std::string fontpath, Alignment align, VerticalAlignment verticalAlignIn)
	: textString(text),
	  fontString(fontpath),
	  fontSizeSpec(Px(static_cast<float>(size))),
	  align(align),
	  verticalAlign(verticalAlignIn),
	  font(*ResourceManager::getFont(fontString)),
	  text(textString, font, size)
{
	type = GUIObject;
	this->x = x;
	this->y = y;
	reloadText();
}

PrimitiveText::PrimitiveText(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	int fontSize,
	std::string text,
	std::string fontpath,
	Alignment align,
	VerticalAlignment verticalAlignIn)
	: PrimitiveText(rectSpec, marginSpec, Px(static_cast<float>(fontSize)), std::move(text), std::move(fontpath), align, verticalAlignIn)
{
}

PrimitiveText::PrimitiveText(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	UnitValue fontSizeSpecIn,
	std::string text,
	std::string fontpath,
	Alignment align,
	VerticalAlignment verticalAlignIn)
	: PrimitiveText(0.0f, 0.0f, 16, std::move(text), std::move(fontpath), align, verticalAlignIn)
{
	fontSizeSpec = fontSizeSpecIn;
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);
}

void PrimitiveText::setFontSizeClamp(float minPx, float maxPx)
{
	minFontSizePx = std::min(minPx, maxPx);
	maxFontSizePx = std::max(minPx, maxPx);
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
	layoutCell = pixelRect;
	layoutResolved = true;

	float fontPx = UnitValueResolver::resolveVertical(fontSizeSpec, parentRect.height);
	const float lo = std::min(minFontSizePx, maxFontSizePx);
	const float hi = std::max(minFontSizePx, maxFontSizePx);
	fontPx = std::min(std::max(fontPx, lo), hi);
	const unsigned charSize = static_cast<unsigned>(std::max(1.0f, std::round(fontPx)));
	text.setCharacterSize(charSize);
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
