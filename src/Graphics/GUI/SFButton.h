#pragma once
#include "SFML/Graphics.hpp"
#include "RenderObject.h"

class SFButton : public RenderObject
{
private:
	sf::RectangleShape* shape;
protected:
	sf::Color color;
	sf::Color baseColor;
	sf::Color hoverColor = sf::Color::Black;
public:

	SFButton();
	virtual ~SFButton() {};

	//virtual
	virtual void render() = 0;
	virtual void draw() = 0;
	virtual void update() = 0;


	//setters
	void setWindow(sf::RenderWindow* window) { this->window = window; }


};

