#pragma once
#include "SFML/Graphics.hpp"
#include "../ResourceManager.h"

enum RenderObjectType {
	EmptyRenderObject = 0,
	BaseRenderObject,
	PrimitiveRenderObject,
	SheetMusicObject,
	GUIObject
};


class RenderObject
{
private:
protected:
	float x, y, width, height;

	sf::RenderWindow* window;
	bool hover = false, unhover = false;

	RenderObjectType type;

public:
	virtual ~RenderObject() {};

	//getters and setters
	bool getHover() { return hover; }
	virtual void setHover(bool hover) { this->hover = hover; }
	
	RenderObjectType getType() { return type; }
	void setWindow(sf::RenderWindow* window) { this->window = window; }
	

	virtual void render() = 0;
	virtual void draw() = 0;
	virtual void update() = 0;

	//returns the x, y width, and height of area
	
	virtual std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
		return std::make_pair(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
	}
	virtual RenderObject& getHoverObject() = 0;

	virtual void onClick() {};
	
	
};

