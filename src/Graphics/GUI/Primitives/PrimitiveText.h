#pragma once
#include "SFML/Graphics.hpp"
#include "../GUIUtilities.h"
#include "../RenderObject.h"
#include "../UnitValue.h"

enum Alignment {ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER};

/** Vertical placement of the glyph bounds inside the resolved RectSpec (after margins). */
enum VerticalAlignment
{
	VERTICAL_ALIGN_TOP,
	VERTICAL_ALIGN_MIDDLE,
	VERTICAL_ALIGN_BOTTOM
};

class PrimitiveText : public RenderObject
{
private:

	std::string textString, fontString;

	/** Pixels or percent of parent height (see UnitValueResolver::resolveVertical). */
	UnitValue fontSizeSpec{ Px(14.0f) };
	/** Applied after resolving `fontSizeSpec` (pixel sizes). Default max avoids runaway % scaling. */
	float minFontSizePx = 1.0f;
	float maxFontSizePx = 512.0f;

	Alignment align;
	VerticalAlignment verticalAlign{ VERTICAL_ALIGN_TOP };

	/** Last resolved rect from RectSpec/margins; used when `layoutResolved` is true. */
	sf::FloatRect layoutCell{};
	bool layoutResolved{ false };

	sf::Font font;
	sf::Text text;


	void reloadText();

public:
	PrimitiveText();
	PrimitiveText(std::string text);
	PrimitiveText(float x, float y, int size, std::string text, std::string fontname = "resources/fonts/SourceCodePro-Regular.ttf",
		Alignment align = ALIGN_LEFT, VerticalAlignment verticalAlign = VERTICAL_ALIGN_TOP);
	/** Positioned via RectSpec/MarginSpec; resolves inside panel content. */
	PrimitiveText(
		const RectSpec& rectSpec,
		const MarginSpec& marginSpec,
		int fontSize,
		std::string text,
		std::string fontpath,
		Alignment align = ALIGN_LEFT,
		VerticalAlignment verticalAlign = VERTICAL_ALIGN_TOP);
	/** `fontSizeSpec`: use `Px(n)` for fixed pixels or `Pct(0.018f)` for 1.8% of parent height (≈18px at 1000px tall). */
	PrimitiveText(
		const RectSpec& rectSpec,
		const MarginSpec& marginSpec,
		UnitValue fontSizeSpec,
		std::string text,
		std::string fontpath,
		Alignment align = ALIGN_LEFT,
		VerticalAlignment verticalAlign = VERTICAL_ALIGN_TOP);

	void render() override;
	void update() override;
	void draw() override;
	void resolveLayout(const sf::FloatRect& parentRect) override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override
	{
		const sf::FloatRect gb = text.getGlobalBounds();
		return std::make_pair(sf::Vector2f(gb.left, gb.top), sf::Vector2f(gb.width, gb.height));
	}
	RenderObject& getHoverObject();


	void setColor(sf::Color color) { text.setFillColor(color); }
	void setText(std::string text) { textString = text; reloadText(); }
	void setPosition(sf::Vector2f position)
	{
		layoutResolved = false;
		this->x = position.x;
		this->y = position.y;
		text.setPosition(position);
	}
	void setAlign(Alignment align) { this->align = align; reloadText(); }
	void setVerticalAlign(VerticalAlignment v) { this->verticalAlign = v; reloadText(); }
	VerticalAlignment getVerticalAlign() const { return verticalAlign; }

	void setFontSizeClamp(float minPx, float maxPx);
	void setFontSizePx(float px);
	float getMinFontSizePx() const { return minFontSizePx; }
	float getMaxFontSizePx() const { return maxFontSizePx; }

	std::string getString() { return this->textString; }

	float getTextWidth(std::string widthText = "");
};

