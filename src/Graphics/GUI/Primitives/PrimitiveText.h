#pragma once
#include "SFML/Graphics.hpp"
#include "../GUIUtilities.h"
#include "../RenderObject.h"

enum Alignment {ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER};

class PrimitiveText : public RenderObject
{
private:

	std::string textString, fontString;

	Alignment align;

	sf::Font font;
	sf::Text text;


	void reloadText();

public:
	PrimitiveText();
	PrimitiveText(std::string text);
	PrimitiveText(float x, float y, int size, std::string text, std::string fontname = "resources/fonts/SourceCodePro-Regular.ttf", Alignment align = ALIGN_LEFT);

	void render() override;
	void update() override;
	void draw() override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {

		float xPos = x;

		switch (align)
		{
		case ALIGN_LEFT:
			xPos = x;
			break;
		case ALIGN_CENTER:
			xPos = x - text.getLocalBounds().width / 2;
			break;
		case ALIGN_RIGHT:
			xPos = x - text.getLocalBounds().width;
			break;
		}

		return std::make_pair(sf::Vector2f(xPos, text.getPosition().y), sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height));
	}
	RenderObject& getHoverObject();


	void setColor(sf::Color color) { text.setFillColor(color); }
	void setText(std::string text) { textString = text; reloadText(); }
	void setPosition(sf::Vector2f position) 
	{ 
		this->x = position.x; this->y = position.y; text.setPosition(position); 
	}
	void setAlign(Alignment align) { this->align = align; reloadText(); }

	std::string getString() { return this->textString; }

	float getTextWidth(std::string widthText = "");
};

