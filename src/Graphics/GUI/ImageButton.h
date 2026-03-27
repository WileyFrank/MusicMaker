#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <string>
#include "GUIButton.h"
#include "GUIUtilities.h"
#include "../ResourceManager.h"


struct ImageButtonColors
{
	sf::Color baseFill;
	sf::Color hoverFill;
	sf::Color activeFill;
	sf::Color outline;
	sf::Color outlineActive;
	/** Multiplied with texture (use white for unchanged artwork). */
	sf::Color imageTint;
	sf::Color imageTintHover;
	sf::Color imageTintActive;
};


class ImageButton : public GUIButton
{
protected:
	sf::RectangleShape* outlineShape;
	sf::RectangleShape* shape;
	float outlineThickness;

	sf::Texture* texture;
	sf::Sprite sprite;
	sf::Color imageTintBase;
	sf::Color imageTintHover;
	sf::Color imageTintActive;
	sf::Color outlineNormalColor;
	sf::Color outlineActiveColor;
	/** Multiplied with the uniform scale after fitting the texture inside the inner rect (default 1). */
	float imageScaleFactor;

	void applyNormalVisuals() override;
	void syncOutlineColor();

private:
	void layoutShapesAndImage();

public:
	ImageButton(
		float xIn,
		float yIn,
		float width,
		float height,
		const std::string& imagePath,
		const ImageButtonColors& colors,
		std::function<void()> onClickAction = nullptr,
		float outlineWidthPx = 2.0f,
		float imageScaleFactor = 1.0f
	);
	ImageButton(
		const RectSpec& rectSpec,
		const MarginSpec& marginSpec,
		const std::string& imagePath,
		const ImageButtonColors& colors,
		std::function<void()> onClickAction = nullptr,
		float outlineWidthPx = 2.0f,
		float imageScaleFactor = 1.0f
	);
	~ImageButton();

	void setImageScaleFactor(float factor);
	float getImageScaleFactor() const { return imageScaleFactor; }

	void update() override;
	void draw() override;
	void render() override;
	void activeDraw() override;
	void resolveLayout(const sf::FloatRect& parentRect) override;
	RenderObject& getHoverObject();
};
