#pragma once

#include "UnitValue.h"

namespace UnitValueResolver
{
inline float resolveHorizontal(const UnitValue& value, float parentWidth)
{
    switch (value.unit)
    {
    case UnitType::Pixels:
        return value.value;
    case UnitType::Percent:
        return parentWidth * value.value;
    default:
        return value.value;
    }
}

inline float resolveVertical(const UnitValue& value, float parentHeight)
{
    switch (value.unit)
    {
    case UnitType::Pixels:
        return value.value;
    case UnitType::Percent:
        return parentHeight * value.value;
    default:
        return value.value;
    }
}
} // namespace UnitValueResolver
