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

inline constexpr UnitValue PctW(float value)
{
    return UnitValue{ value, UnitType::PercentWidth };
}

inline constexpr UnitValue PctW(int value)
{
    return PctW(static_cast<float>(value) / 100.0f);
}

inline constexpr UnitValue PctH(float value)
{
    return UnitValue{ value, UnitType::PercentHeight };
}

inline constexpr UnitValue PctH(int value)
{
    return PctH(static_cast<float>(value) / 100.0f);
}

inline constexpr UnitValue PctInv(float value)
{
    return UnitValue{ value, UnitType::PercentInverse };
}

inline constexpr UnitValue PctInv(int value)
{
    return PctInv(static_cast<float>(value) / 100.0f);
}

inline constexpr UnitValue PctWInv(float value)
{
    return UnitValue{ value, UnitType::PercentWidthInverse };
}

inline constexpr UnitValue PctWInv(int value)
{
    return PctWInv(static_cast<float>(value) / 100.0f);
}

inline constexpr UnitValue PctHInv(float value)
{
    return UnitValue{ value, UnitType::PercentHeightInverse };
}

inline constexpr UnitValue PctHInv(int value)
{
    return PctHInv(static_cast<float>(value) / 100.0f);
}
