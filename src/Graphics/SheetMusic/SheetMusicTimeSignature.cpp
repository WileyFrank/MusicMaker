#include "SheetMusicTimeSignature.h"
#include "../../Music/SheetMusicUtilities.h"

SheetMusicTimeSignature::SheetMusicTimeSignature(TimeSignature timeSignature)
	:numeratorTexture(SheetMusicUtilities::getSignatureTexture(timeSignature.numerator)),
	denominatorTexture(SheetMusicUtilities::getSignatureTexture(timeSignature.numerator))
{

	this->timeSignature = timeSignature;

	numeratorTexture = SheetMusicUtilities::getSignatureTexture(timeSignature.numerator);
	denominatorTexture = SheetMusicUtilities::getSignatureTexture(timeSignature.denominator);
}

SheetMusicTimeSignature::SheetMusicTimeSignature(float x, float y, float height, TimeSignature timeSignature)
	:positionX(x), staffY(y), height(height), 
	timeSignature(timeSignature), numeratorTexture(SheetMusicUtilities::getSignatureTexture(timeSignature.numerator)), 
	denominatorTexture(SheetMusicUtilities::getSignatureTexture(timeSignature.numerator))
{

	numerator.setTexture(numeratorTexture);
	denominator.setTexture(denominatorTexture);

	float numeratorScale = (height / 2) / numerator.getLocalBounds().height;
	float denominatorScale = (height / 2) / denominator.getLocalBounds().height;

	auto max = [](float a, float b) { return a > b ? a : b; };


	numerator.setScale(numeratorScale, numeratorScale);
	denominator.setScale(denominatorScale, denominatorScale);

	width = max(numerator.getLocalBounds().width * numeratorScale, denominator.getLocalBounds().width * denominatorScale);

	numerator.setPosition(sf::Vector2f(positionX, staffY));
	denominator.setPosition(sf::Vector2f(positionX, staffY + (height/2)));
}

void SheetMusicTimeSignature::draw()
{
	window->draw(numerator);
	window->draw(denominator);
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicTimeSignature::getHoverArea()
{
	sf::Vector2f position(
		numerator.getPosition().x - numerator.getOrigin().x,
		staffY);

	sf::Vector2f size(
		numerator.getLocalBounds().width * numerator.getScale().x,
		height);
	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}

RenderObject& SheetMusicTimeSignature::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

	if (GUIUtilities::positionInBounds(mousePosition, this->getHoverArea()))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();

}



void SheetMusicTimeSignature::update()
{
	if (this->hover)
	{
		hoverUpdate();
		unhover = true;
		return;
	}
	if (this->unhover)
	{
		unhover = false;
		unhoverUpdate();
	}
}

void SheetMusicTimeSignature::hoverUpdate()
{
	numerator.setColor(this->hoverColor);
	denominator.setColor(this->hoverColor);
}


void SheetMusicTimeSignature::unhoverUpdate()
{
	numerator.setColor(this->color);
	denominator.setColor(this->color);
}


