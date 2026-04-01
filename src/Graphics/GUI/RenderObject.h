#pragma once
#include "SFML/Graphics.hpp"
#include "../ResourceManager.h"
#include <sstream>
#include "RectSpec.h"

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
	RectSpec rectSpec;
	MarginSpec marginSpec;
	sf::FloatRect resolvedRect;

	sf::RenderWindow* window;
	bool hover = false, unhover = false, active = false, activeOverride = false;

	RenderObjectType type;

public:
	virtual ~RenderObject() {};
	RenderObject() {}

	RenderObject(float x, float y, float width, float height)
		:x(x), y(y), width(width), height(height),
		rectSpec{ Px(x), Px(y), Px(width), Px(height) },
		marginSpec{ Px(0), Px(0), Px(0), Px(0) },
		resolvedRect(x, y, width, height)
	{
	}

	//getters and setters
	bool getHover() { return hover; }
	virtual void setHover(bool hover) { this->hover = hover; }

	virtual void setRectSpec(const RectSpec& spec) { rectSpec = spec; }
	const RectSpec& getRectSpec() const { return rectSpec; }

	void setMarginSpec(const MarginSpec& spec) { marginSpec = spec; }
	const MarginSpec& getMarginSpec() const { return marginSpec; }

	void setResolvedRect(const sf::FloatRect& rect)
	{
		resolvedRect = rect;
		x = rect.left;
		y = rect.top;
		width = rect.width;
		height = rect.height;
	}
	const sf::FloatRect& getResolvedRect() const { return resolvedRect; }
	const sf::FloatRect& getPixelRect() const { return resolvedRect; }

	sf::Vector2f localToAbsolutePoint(const sf::Vector2f& localPoint) const
	{
		return sf::Vector2f(resolvedRect.left + localPoint.x, resolvedRect.top + localPoint.y);
	}

	sf::Vector2f absoluteToLocalPoint(const sf::Vector2f& absolutePoint) const
	{
		return sf::Vector2f(absolutePoint.x - resolvedRect.left, absolutePoint.y - resolvedRect.top);
	}

	sf::FloatRect localToAbsoluteRect(const sf::FloatRect& localRect) const
	{
		return sf::FloatRect(
			resolvedRect.left + localRect.left,
			resolvedRect.top + localRect.top,
			localRect.width,
			localRect.height
		);
	}

	sf::FloatRect absoluteToLocalRect(const sf::FloatRect& absoluteRect) const
	{
		return sf::FloatRect(
			absoluteRect.left - resolvedRect.left,
			absoluteRect.top - resolvedRect.top,
			absoluteRect.width,
			absoluteRect.height
		);
	}

	virtual void resolveLayout(const sf::FloatRect& parentRect)
	{
		const sf::FloatRect unresolvedRect = ResolveRect(parentRect, rectSpec);
		setResolvedRect(ApplyMargin(unresolvedRect, marginSpec, parentRect));
	}
	
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
	virtual void onRightClick() {}
	virtual void keyboardInput(sf::Uint32 input) {}
	virtual void arrowKeyInput(sf::Keyboard::Key key) {}
	virtual void onMouseButtonReleased(sf::Mouse::Button) {}
	
	
};

