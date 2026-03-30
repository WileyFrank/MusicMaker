#include <algorithm>
#include <iostream>

#include "SFML/Graphics.hpp"

#include "RectangleButton.h"

namespace
{
void centerTextInRect(sf::Text& text, float x, float y, float width, float height)
{
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setPosition(
		x + (width * 0.5f) - (bounds.left + bounds.width * 0.5f),
		y + (height * 0.5f) - (bounds.top + bounds.height * 0.5f)
	);
}

int fitCharacterSizeToBounds(sf::Text& text, float width, float height)
{
	const float availableWidth = std::max(1.0f, width - 8.0f);
	const float availableHeight = std::max(1.0f, height - 6.0f);
	int charSize = std::max(10, (int)std::floor(availableHeight * 0.7f));

	text.setCharacterSize((unsigned int)charSize);
	while (charSize > 8 && text.getLocalBounds().width > availableWidth)
	{
		charSize--;
		text.setCharacterSize((unsigned int)charSize);
	}

	return charSize;
}
}

RectangleButton::RectangleButton(
	float xIn,
	float yIn,
	float width,
	float height,
	const RectangleButtonColors& colors,
	std::function<void()> onClickAction,
	float outlineWidthPx
)
{
	// Must match a non-EmptyRenderObject or setActiveHover() will ignore this widget (uninitialized type is often 0 == Empty).
	type = GUIObject;

	this->x = xIn;
	this->y = yIn;
	this->width = width;
	this->height = height;
	this->baseColor = colors.baseFill;
	this->hoverColor = colors.hoverFill;
	this->activeFillColor = colors.activeFill;
	this->textColor = colors.text;
	this->textActiveColor = colors.textActive;
	this->outlineNormalColor = colors.outline;
	this->outlineActiveColor = colors.outlineActive;
	this->color = colors.baseFill;
	this->onClickAction = std::move(onClickAction);

	this->window = nullptr;
	outlineThickness = std::max(0.0f, outlineWidthPx);
	outlineShape = new sf::RectangleShape(sf::Vector2f(width, height));
	outlineShape->setFillColor(colors.outline);
	outlineShape->setPosition(this->x, this->y);

	const float t = outlineThickness;
	const float innerW = std::max(1.0f, width - 2.0f * t);
	const float innerH = std::max(1.0f, height - 2.0f * t);
	shape = new sf::RectangleShape(sf::Vector2f(innerW, innerH));
	shape->setFillColor(colors.baseFill);
	shape->setPosition(this->x + t, this->y + t);

	setRectSpec(RectSpec{ Px(xIn), Px(yIn), Px(width), Px(height) });
	setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
	setResolvedRect(sf::FloatRect(xIn, yIn, width, height));

	font = ResourceManager::getFont("resources/fonts/Dream Orphans Bd.otf");
	if (font == nullptr) {
		std::cerr << "Failed to load font file\n";
		return;
	}


	text.setFont(*font);        // Set the font to the text
	text.setString("Button");  // Set the string to display
	text.setFillColor(colors.text);
	layoutShapesAndText();


}

RectangleButton::RectangleButton(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	const RectangleButtonColors& colors,
	std::function<void()> onClickAction,
	float outlineWidthPx
)
	: RectangleButton(0.0f, 0.0f, 0.0f, 0.0f, colors, std::move(onClickAction), outlineWidthPx)
{
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);
}

RectangleButton::~RectangleButton()
{
	delete outlineShape;
	delete shape;
}

void RectangleButton::layoutShapesAndText()
{
	const float t = outlineThickness;
	const float innerW = std::max(1.0f, width - 2.0f * t);
	const float innerH = std::max(1.0f, height - 2.0f * t);
	outlineShape->setPosition(x, y);
	outlineShape->setSize(sf::Vector2f(width, height));
	shape->setPosition(x + t, y + t);
	shape->setSize(sf::Vector2f(innerW, innerH));
	if (preferredCharacterSize > 0)
	{
		const int maxByHeight = std::max(8, (int)std::floor(std::max(1.0f, innerH - 6.0f)));
		int charSize = std::min(preferredCharacterSize, maxByHeight);
		text.setCharacterSize((unsigned int)charSize);
		const float availableWidth = std::max(1.0f, innerW - 8.0f);
		while (charSize > 8 && text.getLocalBounds().width > availableWidth)
		{
			charSize--;
			text.setCharacterSize((unsigned int)charSize);
		}
	}
	else
	{
		fitCharacterSizeToBounds(text, innerW, innerH);
	}
	centerTextInRect(text, x + t, y + t, innerW, innerH);
}

void RectangleButton::setText(std::string textString, int fontSize)
{
	text.setString(textString);  // Set the string to display
	preferredCharacterSize = std::max(0, fontSize);
	layoutShapesAndText();
}

void RectangleButton::setFontPath(const std::string& fontPath)
{
	sf::Font* newFont = ResourceManager::getFont(fontPath);
	if (newFont == nullptr)
	{
		std::cerr << "Failed to load font file: " << fontPath << "\n";
		return;
	}

	font = newFont;
	text.setFont(*font);
	layoutShapesAndText();
}

void RectangleButton::update()
{
	if (window != nullptr)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);

		const bool over = mousePosition.x > this->x && mousePosition.x < this->x + this->width &&
			mousePosition.y > this->y && mousePosition.y < this->y + this->height;

		if (toggleMode && !active && togglePressed)
		{
			this->color = activeFillColor;
			shape->setFillColor(activeFillColor);
			text.setFillColor(textActiveColor);
			this->hover = over;
		}
		else if (over)
		{
			this->color = hoverColor;
			shape->setFillColor(this->color);
			text.setFillColor(textColor);
			this->hover = true;
		}
		else
		{
			this->color = baseColor;
			shape->setFillColor(this->color);
			text.setFillColor(textColor);
			this->hover = false;
		}
	}
	else
	{
		shape->setFillColor((toggleMode && !active && togglePressed) ? activeFillColor : baseColor);
		text.setFillColor((toggleMode && !active && togglePressed) ? textActiveColor : textColor);
	}
	syncOutlineColor();
}
	

void RectangleButton::syncOutlineColor()
{
	outlineShape->setFillColor(isShowingActiveChrome() ? outlineActiveColor : outlineNormalColor);
}

void RectangleButton::draw()
{
	window->draw(*this->outlineShape);
	window->draw(*this->shape);
	window->draw(this->text);
	return;
}

void RectangleButton::activeDraw()
{
	if (!active)
	{
		return;
	}

	shape->setFillColor(activeFillColor);
	text.setFillColor(textActiveColor);
	draw();
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

void RectangleButton::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	const sf::FloatRect pixelRect = getResolvedRect();
	x = pixelRect.left;
	y = pixelRect.top;
	width = pixelRect.width;
	height = pixelRect.height;

	layoutShapesAndText();
}

void RectangleButton::applyNormalVisuals()
{
	shape->setFillColor(baseColor);
	text.setFillColor(textColor);
	outlineShape->setFillColor(outlineNormalColor);
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
