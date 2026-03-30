#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "GUIButton.h"
#include "GUIUtilities.h"
#include "../ResourceManager.h"


struct RectangleButtonColors
{
	sf::Color baseFill;
	sf::Color hoverFill;
	sf::Color activeFill;
	sf::Color outline;
	sf::Color outlineActive;
	sf::Color text;
	sf::Color textActive;
	/** Border ring; outer bounds of the button match the layout width/height. */
};


class RectangleButton : public GUIButton
{
protected:
	sf::RectangleShape* outlineShape;
	sf::RectangleShape* shape;
	float outlineThickness;

	sf::Font* font;
	sf::Text text;
	sf::Color textColor;
	sf::Color textActiveColor;
	sf::Color outlineNormalColor;
	sf::Color outlineActiveColor;
	int preferredCharacterSize = 0;

	void applyNormalVisuals() override;
	void syncOutlineColor();

private:
	void layoutShapesAndText();

public:
	/** Last argument is optional: pass a lambda at creation, e.g. `[](){ ... }` or `[&]{ foo(); }`. */
	/** outlineWidthPx: border thickness on each side in pixels (inner fill is inset by this amount). */
	RectangleButton(
		float xIn,
		float yIn,
		float width,
		float height,
		const RectangleButtonColors& colors,
		std::function<void()> onClickAction = nullptr,
		float outlineWidthPx = 2.0f
	);
	RectangleButton(
		const RectSpec& rectSpec,
		const MarginSpec& marginSpec,
		const RectangleButtonColors& colors,
		std::function<void()> onClickAction = nullptr,
		float outlineWidthPx = 2.0f
	);
	~RectangleButton();

	//setters
	void setText(std::string textString, int fontSize = 0);
	void setFontPath(const std::string& fontPath);

	void update() override;
	void draw() override;
	void render() override;
	void activeDraw() override;
	void resolveLayout(const sf::FloatRect& parentRect) override;
	RenderObject& getHoverObject();

};

