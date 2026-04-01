#pragma once

#include <algorithm>

#include "SFML/Graphics.hpp"

#include "UnitValue.h"
#include "UnitValueResolver.h"

struct RectSpec
{
    UnitValue x;
    UnitValue y;
    UnitValue width;
    UnitValue height;
};

struct MarginSpec
{
    UnitValue left;
    UnitValue top;
    UnitValue right;
    UnitValue bottom;
};

inline sf::FloatRect ResolveRect(const sf::FloatRect& parentRect, const RectSpec& childRect)
{
    const float resolvedX = parentRect.left + UnitValueResolver::resolveHorizontal(childRect.x, parentRect.width, parentRect.height);
    const float resolvedY = parentRect.top + UnitValueResolver::resolveVertical(childRect.y, parentRect.height, parentRect.width);
    const float resolvedWidth = UnitValueResolver::resolveHorizontal(childRect.width, parentRect.width, parentRect.height);
    const float resolvedHeight = UnitValueResolver::resolveVertical(childRect.height, parentRect.height, parentRect.width);

    return sf::FloatRect(resolvedX, resolvedY, resolvedWidth, resolvedHeight);
}

inline sf::FloatRect ApplyMargin(
    const sf::FloatRect& rect,
    const MarginSpec& margin,
    const sf::FloatRect& parentRect
) {
    const float marginLeft = UnitValueResolver::resolveHorizontal(margin.left, parentRect.width, parentRect.height);
    const float marginTop = UnitValueResolver::resolveVertical(margin.top, parentRect.height, parentRect.width);
    const float marginRight = UnitValueResolver::resolveHorizontal(margin.right, parentRect.width, parentRect.height);
    const float marginBottom = UnitValueResolver::resolveVertical(margin.bottom, parentRect.height, parentRect.width);

    const float resolvedLeft = rect.left + marginLeft;
    const float resolvedTop = rect.top + marginTop;
    const float resolvedWidth = std::max(0.0f, rect.width - marginLeft - marginRight);
    const float resolvedHeight = std::max(0.0f, rect.height - marginTop - marginBottom);

    return sf::FloatRect(resolvedLeft, resolvedTop, resolvedWidth, resolvedHeight);
}
