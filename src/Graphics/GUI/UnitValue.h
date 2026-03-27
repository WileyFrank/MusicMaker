#pragma once

#include "UnitType.h"

struct UnitValue
{
    float value = 0.0f;
    UnitType unit = UnitType::Pixels;
};

inline constexpr UnitValue Px(float value)
{
    return UnitValue{ value, UnitType::Pixels };
}

inline constexpr UnitValue Px(int value)
{
    return Px(static_cast<float>(value));
}

inline constexpr UnitValue Pct(float value)
{
    return UnitValue{ value, UnitType::Percent };
}

inline constexpr UnitValue Pct(int value)
{
    return Pct(static_cast<float>(value) / 100.0f);
}
