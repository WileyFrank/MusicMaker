#include "PrimitiveRoundRectangle.h"

PrimitiveRoundRectangle::PrimitiveRoundRectangle(float x, float y, float width, float height, sf::Color color, sf::Color outlineColor, float outline, float cornerRadius, int cornerPointCount)
    :shape(4 * cornerPointCount + 8), width(width), height(height), x(x), y(y),
    color(color), outlineColor(outlineColor)
{
    std::size_t pointIndex = 0;

    // Helper lambda to set points in the correct order
    auto setPoint = [&](float x, float y) {
        shape.setPoint(pointIndex++, sf::Vector2f(x, y));
        };

    // Top left corner
    std::vector<std::pair<float, float>> points;
    std::vector<std::pair<float, float>> anchorPoints;
    points.push_back(std::make_pair(width - cornerRadius, cornerRadius));
    points.push_back(std::make_pair(cornerRadius, cornerRadius));
    points.push_back(std::make_pair(cornerRadius, height - cornerRadius));
    points.push_back(std::make_pair(width - cornerRadius, height - cornerRadius));

    anchorPoints.push_back(std::make_pair(width, cornerRadius));
    anchorPoints.push_back(std::make_pair(cornerRadius, 0.0f));
    anchorPoints.push_back(std::make_pair(0.0f, height - cornerRadius));
    anchorPoints.push_back(std::make_pair(width - cornerRadius, height));

    anchorPoints.push_back(std::make_pair(width - cornerRadius, 0.0f));
    anchorPoints.push_back(std::make_pair(0.0f, cornerRadius));
    anchorPoints.push_back(std::make_pair(cornerRadius, height));
    anchorPoints.push_back(std::make_pair(width, height - cornerRadius));


    float pi = 3.14159265f;

    for (int i = 0; i < 4; i++)
    {
        setPoint(anchorPoints[i].first, anchorPoints[i].second);
        for (int j = 0; j < (float)cornerPointCount; j++)
        {
            float angle = ((90.0f * i) + (j * (90.0f / cornerPointCount))) * pi / 180.0f;
            float x = points[i].first + cos(angle) * cornerRadius;
            float y = points[i].second - sin(angle) * cornerRadius;

            setPoint(x, y);
        }
        setPoint(anchorPoints[i + 4].first, anchorPoints[i + 4].second);
    }

    // Complete the shape by setting the last point equal to the first
    shape.setPoint(pointIndex - 1, shape.getPoint(0));

    shape.setFillColor(color);
    shape.setOutlineColor(outlineColor);
    shape.setOutlineThickness(outline);
    shape.setPosition(x, y); 

}

void PrimitiveRoundRectangle::render()
{
    update();
    draw();
}

void PrimitiveRoundRectangle::update()
{
}

void PrimitiveRoundRectangle::draw()
{
    if (window != nullptr)
    {
        window->draw(shape);
    }
}

RenderObject& PrimitiveRoundRectangle::getHoverObject()
{
	auto mouse = sf::Mouse::getPosition(*window);

	if (GUIUtilities::positionInBounds(mouse, this->getHoverArea()))
	{
		return *this;
	}
    return *GUIUtilities::getEmptyRenderObject();
}
