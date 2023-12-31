#pragma once

#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "../GUI/RenderObject.h"
#include "../GUI/GUIUtilities.h"
#include "../GUI/GUIPanel.h"

class SheetMusicElement : public RenderObject
{
protected:

	GUIPanel* hoverPanel;
	std::vector<RenderObject*> objects;

	sf::RenderWindow* window;
	std::chrono::steady_clock::time_point hoverStart;

	bool runHoverAction = true;
	bool overrideAction = false;


public:
	SheetMusicElement() :window(nullptr) { type = SheetMusicObject; }
	virtual ~SheetMusicElement() = default;

	GUIPanel* addPanel(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor = sf::Color(0, 1, 2), float outlineWidth = 0);

	virtual void draw() = 0;
	virtual void update() {};
	virtual void render(); 
	
	virtual std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override = 0;

	virtual RenderObject& getHoverObject() 
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

		if (GUIUtilities::positionInBounds(mousePosition, this->getHoverArea()))
		{
			return *this;
		}

		return *GUIUtilities::getEmptyRenderObject();

	}
	

	virtual void setWindow(sf::RenderWindow* window) { this->window = window; }
	void setOverrideAction(bool action) { this->overrideAction = action; }
	void setHoverStart(std::chrono::steady_clock::time_point hoverStart) { this->hoverStart = hoverStart; }


	void drawBoundingBox()
	{
		if (window != nullptr)
		{
			auto bounds = getHoverArea();
			sf::RectangleShape rect(bounds.second);
			rect.setPosition(bounds.first);
			rect.setOutlineColor(sf::Color(255, 0, 0));
			rect.setFillColor(sf::Color(0, 0, 0, 0));
			rect.setOutlineThickness(2);

			window->draw(rect);
		}
	}
};

