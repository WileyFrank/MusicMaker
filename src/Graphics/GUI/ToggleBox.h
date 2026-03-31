#pragma once
#include <functional>
#include "RenderObject.h"
#include "GUIUtilities.h"


class ToggleBox :
    public RenderObject
{
private:
    bool boxState;
    int outlineWidth;

    sf::RectangleShape outBox, selectionBox;
    sf::Color backgroundColor, outlineColor, selectionColor, backgroundHoverColor, selectionHoverColor;

    std::function<void(bool)> onStateChanged;

public:
    ToggleBox(
        const RectSpec& rectSpec,
        const MarginSpec& marginSpec,
        sf::Color background,
        sf::Color outlineColor,
        sf::Color selectionColor,
        sf::Color backgroundHoverColor,
        sf::Color selectionHoverColor,
        std::function<void(bool)> stateChanged = nullptr
    );
        
    bool getState() { return boxState; }

    /** Sets checked state and idle colors. Pass notifyCallback true to invoke onStateChanged (e.g. programmatic sync). */
    void setChecked(bool checked, bool notifyCallback = false);

    void resolveLayout(const sf::FloatRect& parentRect) override;
    void update() override;
    void draw() override;
    void hoverDraw() override;
    void activeDraw() override;

    void setHoverstate();
    void setUnhover();

    void setActive() override;
    void setInactive() override;

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
        const sf::FloatRect pixelRect = getResolvedRect();
        return std::make_pair(
            sf::Vector2f(pixelRect.left, pixelRect.top),
            sf::Vector2f(pixelRect.width, pixelRect.height)
        );
    }
    RenderObject& getHoverObject();

    virtual void onClick();

private:
    void applyIdleVisuals();
    void notifyStateChanged();
};

