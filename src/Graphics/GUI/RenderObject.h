#pragma once
#include "SFML/Graphics.hpp"
#include "../ResourceManager.h"
#include <sstream>

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
	bool hover = false, unhover = false, active = false, activeOverride = false;

	RenderObjectType type;

public:
	virtual ~RenderObject() {};
	RenderObject() {}

	RenderObject(float x, float y, float width, float height)
		:x(x), y(y), width(width), height(height) {}

	//getters and setters
	bool getHover() { return hover; }
	virtual void setHover(bool hover) { this->hover = hover; }
	
	RenderObjectType getType() { return type; }
	virtual void setWindow(sf::RenderWindow* window) { this->window = window; }
	 

	virtual void render();
	virtual void draw() = 0;
	virtual void activeDraw() {};
	virtual void hoverDraw() {};
	
	virtual void update() = 0;

	virtual void setHoverstate() {};
	virtual void setUnhover() {};

	virtual void setActive() {}
	virtual void setInactive() {}

	bool getActive() { return active; }
	bool getActiveOverride() { return activeOverride; }

	//returns the x, y width, and height of area
	
	virtual std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
		return std::make_pair(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
	}
	virtual RenderObject& getHoverObject() = 0;
	virtual RenderObject& getActiveObject() { return *this; }
	virtual void onClick() {}
	virtual void keyboardInput(sf::Uint32 input) {}
	virtual void arrowKeyInput(sf::Keyboard::Key key) {}
	
	
};

