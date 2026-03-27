#pragma once

#include "SFML/Graphics.hpp"

namespace Theme
{
    // Background
    static const sf::Color Background = sf::Color(0, 3, 25);

    // Structural surfaces
    static const sf::Color Panel = sf::Color(11, 0, 44);
    static const sf::Color PanelAlt = sf::Color(14, 10, 40);
    static const sf::Color ElevatedPanel = sf::Color(28, 22, 68);
    static const sf::Color SectionBlock = sf::Color(24, 24, 58);

    // Interactive surfaces
    static const sf::Color Control = sf::Color(26, 18, 78);
    static const sf::Color ControlHover = sf::Color(38, 28, 108);
    static const sf::Color ControlPressed = sf::Color(52, 40, 138);
    static const sf::Color ControlDisabled = sf::Color(32, 28, 50);

    // Borders / accents
    static const sf::Color BorderSubtle = sf::Color(44, 42, 86);
    static const sf::Color BorderPanel = sf::Color(58, 54, 112);
    static const sf::Color Accent = sf::Color(76, 62, 196);
    static const sf::Color AccentHover = sf::Color(96, 82, 216);
    static const sf::Color AccentBright = sf::Color(94, 150, 255);

    // Text
    static const sf::Color TextPrimary = sf::Color(247, 235, 236);
    static const sf::Color TextSecondary = sf::Color(212, 209, 255);
    static const sf::Color TextMuted = sf::Color(190, 188, 216);
    static const sf::Color TextCool = sf::Color(150, 150, 255);
    static const sf::Color TextDim = sf::Color(110, 116, 160);

    // Music elements
    static const sf::Color StaffLine = sf::Color(108, 110, 235);
    static const sf::Color StaffHover = sf::Color(255, 200, 255);
    static const sf::Color Clef = sf::Color(170, 172, 255);
    static const sf::Color KeySig = sf::Color(170, 172, 255);
    static const sf::Color Note = sf::Color(94, 150, 255);
    static const sf::Color NoteHover = sf::Color(55, 220, 255);
    static const sf::Color NoteSelected = sf::Color(140, 210, 255);

    // Tooltips
    static const sf::Color TooltipFill = sf::Color(24, 18, 72);
    static const sf::Color TooltipBorder = sf::Color(70, 64, 138);
    static const sf::Color TooltipText = sf::Color(230, 228, 255);
    static const sf::Color TooltipSecondary = sf::Color(200, 198, 236);
}
