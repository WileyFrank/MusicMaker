#pragma once
#include "SFML/Graphics.hpp"
#include "SFButton.h"
#include "GUIUtilities.h"
#include "../ResourceManager.h"


class RectangleButton : public SFButton
{
protected:
	sf::RectangleShape* shape;
	float width, height;

	sf::Font* font;
	sf::Text text;
public:
	RectangleButton(float xIn, float yIn, float width, float height, sf::Color colorIn);
	~RectangleButton();

	//setters
	void setText(std::string textString);

	void update() override;
	void draw() override;
	void render() override;

	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
		return std::make_pair(sf::Vector2f(shape->getPosition().x, shape->getPosition().y), sf::Vector2f(shape->getLocalBounds().width, shape->getLocalBounds().height));
	}
	RenderObject& getHoverObject();

};

