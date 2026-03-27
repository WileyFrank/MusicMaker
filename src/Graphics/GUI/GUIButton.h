#pragma once
#include <functional>
#include <utility>
#include "SFML/Graphics.hpp"
#include "SFButton.h"

/**
 * Shared behavior for rectangle and image buttons (toggle, click, unselect).
 * Use GUIButton* when either concrete type is acceptable.
 */
class GUIButton : public SFButton
{
protected:
	bool toggleMode = false;
	bool togglePressed = false;
	std::function<void()> onClickAction;
	sf::Color activeFillColor;

	virtual void applyNormalVisuals() = 0;

	/** True when inner fill uses active/selected styling (pressed, selected, or toggle latched). */
	bool isShowingActiveChrome() const { return active || (toggleMode && !active && togglePressed); }

public:
	~GUIButton() override = default;

	void setActive() override;
	void setInactive() override;
	void onClick() override;
	void onMouseButtonReleased(sf::Mouse::Button button) override;
	void setUnselected();

	void setOnClick(std::function<void()> action) { onClickAction = std::move(action); }
	void setToggleMode(bool enabled) { toggleMode = enabled; }

	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() {
		const sf::FloatRect pixelRect = getPixelRect();
		return std::make_pair(
			sf::Vector2f(pixelRect.left, pixelRect.top),
			sf::Vector2f(pixelRect.width, pixelRect.height)
		);
	}
};
