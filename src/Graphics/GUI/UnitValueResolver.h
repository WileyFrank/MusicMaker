#pragma once

#include "UnitValue.h"

namespace UnitValueResolver
{
inline float resolveHorizontal(const UnitValue& value, float parentWidth, float parentHeight = -1.0f)
{
    if (parentHeight < 0.0f)
    {
        parentHeight = parentWidth;
    }
    switch (value.unit)
    {
    case UnitType::Pixels:
        return value.value;
    case UnitType::Percent:
        return parentWidth * value.value;
    case UnitType::PercentWidth:
        return parentWidth * value.value;
    case UnitType::PercentHeight:
        return parentHeight * value.value;
    case UnitType::PercentInverse:
        return parentWidth * (1.0f - value.value);
    case UnitType::PercentWidthInverse:
        return parentWidth - (parentWidth * value.value);
    case UnitType::PercentHeightInverse:
        // In horizontal context: full width minus percent of parent height.
        return parentWidth - (parentHeight * value.value);
    default:
        return value.value;
    }
}

inline float resolveVertical(const UnitValue& value, float parentHeight, float parentWidth = -1.0f)
{
    if (parentWidth < 0.0f)
    {
        parentWidth = parentHeight;
    }
    switch (value.unit)
    {
    case UnitType::Pixels:
        return value.value;
    case UnitType::Percent:
        return parentHeight * value.value;
    case UnitType::PercentWidth:
        return parentWidth * value.value;
    case UnitType::PercentHeight:
        return parentHeight * value.value;
    case UnitType::PercentInverse:
        return parentHeight * (1.0f - value.value);
    case UnitType::PercentWidthInverse:
        // In vertical context: full height minus percent of parent width.
        return parentHeight - (parentWidth * value.value);
    case UnitType::PercentHeightInverse:
        return parentHeight - (parentHeight * value.value);
    default:
        return value.value;
    }
}
} // namespace UnitValueResolver
