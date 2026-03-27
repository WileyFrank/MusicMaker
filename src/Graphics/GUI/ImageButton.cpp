#include <algorithm>
#include <iostream>

#include "SFML/Graphics.hpp"

#include "ImageButton.h"

namespace
{
constexpr float kImagePaddingPx = 4.0f;
}

ImageButton::ImageButton(
	float xIn,
	float yIn,
	float width,
	float height,
	const std::string& imagePath,
	const ImageButtonColors& colors,
	std::function<void()> onClickAction,
	float outlineWidthPx,
	float imageScaleFactorIn
)
{
	type = GUIObject;

	this->x = xIn;
	this->y = yIn;
	this->width = width;
	this->height = height;
	this->baseColor = colors.baseFill;
	this->hoverColor = colors.hoverFill;
	this->activeFillColor = colors.activeFill;
	this->imageTintBase = colors.imageTint;
	this->imageTintHover = colors.imageTintHover;
	this->imageTintActive = colors.imageTintActive;
	this->outlineNormalColor = colors.outline;
	this->outlineActiveColor = colors.outlineActive;
	this->color = colors.baseFill;
	this->onClickAction = std::move(onClickAction);
	this->imageScaleFactor = imageScaleFactorIn;

	this->window = nullptr;
	outlineThickness = std::max(0.0f, outlineWidthPx);
	outlineShape = new sf::RectangleShape(sf::Vector2f(width, height));
	outlineShape->setFillColor(colors.outline);
	outlineShape->setPosition(this->x, this->y);

	const float t = outlineThickness;
	const float innerW = std::max(1.0f, width - 2.0f * t);
	const float innerH = std::max(1.0f, height - 2.0f * t);
	shape = new sf::RectangleShape(sf::Vector2f(innerW, innerH));
	shape->setFillColor(colors.baseFill);
	shape->setPosition(this->x + t, this->y + t);

	texture = ResourceManager::getTexture(imagePath);
	if (texture != nullptr)
	{
		sprite.setTexture(*texture);
		sprite.setColor(colors.imageTint);
	}

	setRectSpec(RectSpec{ Px(xIn), Px(yIn), Px(width), Px(height) });
	setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
	setResolvedRect(sf::FloatRect(xIn, yIn, width, height));

	layoutShapesAndImage();
}

ImageButton::ImageButton(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	const std::string& imagePath,
	const ImageButtonColors& colors,
	std::function<void()> onClickAction,
	float outlineWidthPx,
	float imageScaleFactorIn
)
	: ImageButton(0.0f, 0.0f, 0.0f, 0.0f, imagePath, colors, std::move(onClickAction), outlineWidthPx, imageScaleFactorIn)
{
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);
}

ImageButton::~ImageButton()
{
	delete outlineShape;
	delete shape;
}

void ImageButton::setImageScaleFactor(float factor)
{
	imageScaleFactor = factor;
	layoutShapesAndImage();
}

void ImageButton::layoutShapesAndImage()
{
	const float t = outlineThickness;
	const float innerW = std::max(1.0f, width - 2.0f * t);
	const float innerH = std::max(1.0f, height - 2.0f * t);
	outlineShape->setPosition(x, y);
	outlineShape->setSize(sf::Vector2f(width, height));
	shape->setPosition(x + t, y + t);
	shape->setSize(sf::Vector2f(innerW, innerH));

	if (texture == nullptr)
	{
		return;
	}

	const unsigned texW = texture->getSize().x;
	const unsigned texH = texture->getSize().y;
	if (texW == 0 || texH == 0)
	{
		return;
	}

	const float availW = std::max(1.0f, innerW - 2.0f * kImagePaddingPx);
	const float availH = std::max(1.0f, innerH - 2.0f * kImagePaddingPx);
	const float tw = static_cast<float>(texW);
	const float th = static_cast<float>(texH);
	const float fitScale = std::min(availW / tw, availH / th);
	const float scale = fitScale * imageScaleFactor;
	sprite.setScale(scale, scale);

	const float scaledW = tw * scale;
	const float scaledH = th * scale;
	sprite.setPosition(
		x + t + (innerW - scaledW) * 0.5f,
		y + t + (innerH - scaledH) * 0.5f
	);
}

void ImageButton::update()
{
	if (window != nullptr)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);

		const bool over = mousePosition.x > this->x && mousePosition.x < this->x + this->width &&
			mousePosition.y > this->y && mousePosition.y < this->y + this->height;

		if (toggleMode && !active && togglePressed)
		{
			this->color = activeFillColor;
			shape->setFillColor(activeFillColor);
			sprite.setColor(imageTintActive);
			this->hover = over;
		}
		else if (over)
		{
			this->color = hoverColor;
			shape->setFillColor(this->color);
			sprite.setColor(imageTintHover);
			this->hover = true;
		}
		else
		{
			this->color = baseColor;
			shape->setFillColor(this->color);
			sprite.setColor(imageTintBase);
			this->hover = false;
		}
	}
	else
	{
		shape->setFillColor((toggleMode && !active && togglePressed) ? activeFillColor : baseColor);
		sprite.setColor((toggleMode && !active && togglePressed) ? imageTintActive : imageTintBase);
	}
	syncOutlineColor();
}

void ImageButton::syncOutlineColor()
{
	outlineShape->setFillColor(isShowingActiveChrome() ? outlineActiveColor : outlineNormalColor);
}

void ImageButton::draw()
{
	window->draw(*this->outlineShape);
	window->draw(*this->shape);
	if (texture != nullptr)
	{
		window->draw(this->sprite);
	}
}

void ImageButton::activeDraw()
{
	if (!active)
	{
		return;
	}

	shape->setFillColor(activeFillColor);
	sprite.setColor(imageTintActive);
	syncOutlineColor();
	draw();
}

void ImageButton::render()
{
	if (window != nullptr)
	{
		update();
		draw();
		return;
	}

	std::cerr << "Error: Attempted to draw ImageButton when window is uninitialized\n";
}

void ImageButton::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	const sf::FloatRect pixelRect = getResolvedRect();
	x = pixelRect.left;
	y = pixelRect.top;
	width = pixelRect.width;
	height = pixelRect.height;

	layoutShapesAndImage();
}

void ImageButton::applyNormalVisuals()
{
	shape->setFillColor(baseColor);
	sprite.setColor(imageTintBase);
	outlineShape->setFillColor(outlineNormalColor);
}

RenderObject& ImageButton::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	auto hoverArea = this->getHoverArea();

	if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}
