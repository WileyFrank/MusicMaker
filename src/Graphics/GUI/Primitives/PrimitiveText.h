#pragma once
#include "SFML/Graphics.hpp"
#include "../GUIUtilities.h"
#include "../RenderObject.h"

enum Alignment {ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER};

class PrimitiveText : public RenderObject
{
private:
	float x, y;

	std::string textString, fontString;

	Alignment align;

	sf::Font font;
	sf::Text text;


	void reloadText();

public:
	PrimitiveText();
	PrimitiveText(std::string text);
	PrimitiveText(float x, float y, int size, std::string text, std::string fontname, Alignment align = ALIGN_LEFT);

	void render() override;
	void update() override;
	void draw() override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {
		return std::make_pair(sf::Vector2f(text.getPosition().x, text.getPosition().y), sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height));
	}
	RenderObject& getHoverObject();


	void setColor(sf::Color color) { text.setFillColor(color); }
	void setText(std::string text) { textString = text; reloadText(); }
};

