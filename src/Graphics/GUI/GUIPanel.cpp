#include "GUIPanel.h"
#include <iostream>
#include <algorithm>

GUIPanel::GUIPanel()
	: currentX(5.0f), currentY(5.0f),
	backgroundColor(sf::Color(255,255,255)), outlineColor(sf::Color(255, 255, 255))
{
	type = GUIObject;

	this->x = 0;
	this->y = 0;
	this->width = 100;
	this->height = 100;

	auto rect = new Rectangle(x, y, width, height, backgroundColor, outlineColor);
	rect->setRectSpec(RectSpec{ Px(0), Px(0), Pct(1.0f), Pct(1.0f) });
	rect->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
	backgroundShape = rect;

	addChild(rect);
}

GUIPanel::GUIPanel(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline, float cornerSize)
	:currentX(5.0f), currentY(5.0f),
	backgroundColor(color), outlineColor(outlineColor)
{
	setRectSpec(RectSpec{ Px(x), Px(y), Px(width), Px(height) });
	setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
	setResolvedRect(sf::FloatRect(x, y, width, height));
	setPadding(0.0f);

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	if (cornerSize <= 0.0f)
	{
		Rectangle* panel = new Rectangle(x, y, width, height, color, outlineColor, outline);
		panel->setRectSpec(RectSpec{ Px(0), Px(0), Pct(1.0f), Pct(1.0f) });
		panel->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
		backgroundShape = panel;
		addChild(panel);
	}
	else
	{
		PrimitiveRoundRectangle* panel = new PrimitiveRoundRectangle(x, y, width, height, color, outlineColor, outline, cornerSize, 10);
		panel->setRectSpec(RectSpec{ Px(0), Px(0), Pct(1.0f), Pct(1.0f) });
		panel->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
		backgroundShape = panel;
		addChild(panel);
	}
}

GUIPanel::GUIPanel(const RectSpec& rectSpec, sf::Color color, sf::Color outlineColor, float outline, float cornerSize, MarginSpec marginSpec)
	: currentX(5.0f), currentY(5.0f),
	backgroundColor(color), outlineColor(outlineColor)
{
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);
	setPadding(0.0f);

	if (cornerSize <= 0.0f)
	{
		Rectangle* panel = new Rectangle(x, y, width, height, color, outlineColor, outline);
		panel->setRectSpec(RectSpec{ Pct(0.0f), Pct(0.0f), Pct(1.0f), Pct(1.0f) });
		panel->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
		backgroundShape = panel;
		addChild(panel);
	}
	else
	{
		PrimitiveRoundRectangle* panel = new PrimitiveRoundRectangle(x, y, width, height, color, outlineColor, outline, cornerSize, 10);
		panel->setRectSpec(RectSpec{ Pct(0.0f), Pct(0.0f), Pct(1.0f), Pct(1.0f) });
		panel->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
		backgroundShape = panel;
		addChild(panel);
	}
}

void GUIPanel::setRectSpec(const RectSpec& spec)
{
	Container::setRectSpec(spec);
}

void GUIPanel::addText(std::string text, int size, sf::Color color)
{
	PrimitiveText* textObject = new PrimitiveText(0.0f, 0.0f, size, text, "resources/fonts/Sansation_Regular.ttf", ALIGN_LEFT);
	textObject->setRectSpec(RectSpec{ Px(currentX), Px(currentY), Px(0), Px(0) });
	textObject->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });

	const int lineCount = std::max(1, 1 + static_cast<int>(std::count(text.begin(), text.end(), '\n')));
	const float lineHeight = static_cast<float>(size) * 1.35f;
	currentY += (lineCount * lineHeight) + padding;
	textObject->setColor(color);
	textObject->setWindow(window);

	addChild(textObject);
}

void GUIPanel::resolveChildrenLayout()
{
	// Background fills the full outer panel bounds (not the padded content rect).
	if (backgroundShape != nullptr)
	{
		backgroundShape->resolveLayout(getResolvedRect());
	}

	// All non-background children resolve inside padded content.
	const sf::FloatRect contentRect = getContentRect();
	for (auto* child : getChildren())
	{
		if (child == nullptr || child == backgroundShape)
		{
			continue;
		}

		child->resolveLayout(contentRect);
	}
}

void GUIPanel::resolveLayout(const sf::FloatRect& parentRect)
{
	Container::resolveLayout(parentRect);

	const sf::FloatRect panelRect = getResolvedRect();
	/*std::cout
		<< "[GUIPanel::resolveLayout] x=" << panelRect.left
		<< " y=" << panelRect.top
		<< " w=" << panelRect.width
		<< " h=" << panelRect.height
		<< std::endl;*/
}

RenderObject& GUIPanel::getHoverObject()
{
	return Container::getHoverObject();
}
