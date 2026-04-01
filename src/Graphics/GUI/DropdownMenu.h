#pragma once
#include <string>
#include <functional>
#include "GUIUtilities.h"
#include "RenderObject.h"
#include "../ResourceManager.h"
#include "Primitives/PrimitiveText.h"

template <typename T>
class DropdownMenu : public RenderObject {

private:

    int  fontSize;
    float optionHeight, margin, textY;

    std::string defaultText;
    std::vector<T> options;
    int selectedIndex = -1;
    int hoverIndex = -1;
    bool isExpanded = false; // To track if the dropdown is expanded
    size_t optionDisplayCount; // The amount of options to display when expanded
    size_t startDisplayIndex;
    sf::RectangleShape box; // Main dropdown box
    sf::Sprite arrowSprite;
    sf::Texture* arrowTexture{ nullptr };
    PrimitiveText text;
    std::vector<sf::RectangleShape> optionBoxes; // Shapes for each option
    std::vector<PrimitiveText*> optionTexts; // Text for each option
    sf::Color backgroundColor, textColor, activeBackgroundColor, activeTextColor;

    std::function<void(DropdownMenu<T>*, const T&, int)> onSelectionChanged;

    /** Captured when opening; restored when closing without choosing an option (e.g. click outside). */
    std::string textAtOpen;
    int selectedIndexAtOpen = -1;
    bool closeCommitsSelection = false;

    void updateKeyboardHighlight()
    {
        for (int i = 0; i < static_cast<int>(optionDisplayCount) && i < static_cast<int>(optionBoxes.size()); ++i)
        {
            optionBoxes[i].setFillColor(i == hoverIndex ? activeBackgroundColor : backgroundColor);
        }
    }

    void initializeDropdownMenu() {
        optionHeight = height;
        margin = height / 2.0f;

        // Main dropdown appearance
        box.setSize(sf::Vector2f(width, height));
        box.setPosition(sf::Vector2f(x, y));
        box.setFillColor(backgroundColor);

        // Load and position dropdown arrow at far right of main box.
        if (arrowTexture == nullptr)
        {
            arrowTexture = ResourceManager::getTexture("resources/images/GUI/dropdownArrow.png", true);
            if (arrowTexture != nullptr)
            {
                arrowSprite.setTexture(*arrowTexture);
            }
        }
        if (arrowTexture != nullptr)
        {
            const float padding = 4.0f;
            const float boxH = height;
            const unsigned texW = arrowTexture->getSize().x;
            const unsigned texH = arrowTexture->getSize().y;
            if (texW > 0 && texH > 0 && boxH > 0.0f)
            {
                const float tw = static_cast<float>(texW);
                const float th = static_cast<float>(texH);
                const float fitScale = (boxH - 2.0f * padding) / th;
                const float scale = std::max(0.0f, fitScale * 0.75f);
                arrowSprite.setScale(scale, scale);

                const float scaledW = tw * scale;
                const float scaledH = th * scale;
                const float px = std::round(x + width - padding - scaledW);
                const float py = std::round(y + (boxH - scaledH) * 0.5f);
                arrowSprite.setPosition(px, py);
            }
        }

        textY = y + height - (fontSize * 1.375f);
        text.setText(defaultText);
        text.setPosition(sf::Vector2f(x + margin / 2, textY));

        // Initialize each option box and text based on the options
        optionBoxes.clear();
        float boxY;
        std::string optionString = "---";
        for (size_t i = 0; i < optionDisplayCount; ++i) {
            boxY = y + height + optionHeight * (i);
            sf::RectangleShape optionBox(sf::Vector2f(width, height));
            
            optionBox.setPosition(sf::Vector2f(x, boxY));
            optionBox.setFillColor(backgroundColor);
            optionBoxes.push_back(optionBox);
            textY = boxY + optionHeight - (fontSize * 1.375f);
            if (i >= optionTexts.size()) {
                PrimitiveText* optionText = new PrimitiveText(x, textY, fontSize, optionString);
                optionText->setText("----");
                optionText->setColor(textColor);
                optionTexts.push_back(optionText);
            }
            optionTexts[i]->setPosition(sf::Vector2f(x + margin / 2, textY));
            optionTexts[i]->setColor(textColor);
        }

        for (size_t i = 0; i < options.size(); i++)
        {
            optionTexts[i]->setText(options[i]);
        }

        setInactive();
    }
public:
    DropdownMenu(float x, float y, float width, float height, int fontSize, int optionDisplayCount, const std::vector<T>& options,
        sf::Color backgroundColor = sf::Color(200, 200, 200), sf::Color textColor = sf::Color(50, 50, 50),
        sf::Color activeBackgroundColor = sf::Color(255, 255, 255), sf::Color activeTextColor = sf::Color(0, 0, 0),
        const std::string& placeholderText = "Select an Option",
        std::function<void(DropdownMenu<T>*, const T&, int)> onSelectionChanged = nullptr)
        : options(options),
          defaultText(placeholderText),
          fontSize(fontSize),
          backgroundColor(backgroundColor),
          textColor(textColor),
          activeBackgroundColor(activeBackgroundColor),
          activeTextColor(activeTextColor),
          onSelectionChanged(std::move(onSelectionChanged)),
          text(x, y, fontSize, defaultText),
          margin(height / 2),
          optionDisplayCount(optionDisplayCount),
          optionHeight(height)
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->width = 0.0f;
        this->height = 0.0f;
        setRectSpec(RectSpec{ Px(x), Px(y), Px(width), Px(height) });
        setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        setResolvedRect(sf::FloatRect(x, y, width, height));
        initializeDropdownMenu();
        this->type = GUIObject;
    }

    DropdownMenu(const RectSpec& rectSpec, const MarginSpec& marginSpec, int fontSize, int optionDisplayCount, const std::vector<T>& options,
        sf::Color backgroundColor = sf::Color(200, 200, 200), sf::Color textColor = sf::Color(50, 50, 50),
        sf::Color activeBackgroundColor = sf::Color(255, 255, 255), sf::Color activeTextColor = sf::Color(0, 0, 0),
        const std::string& placeholderText = "Select an Option",
        std::function<void(DropdownMenu<T>*, const T&, int)> onSelectionChanged = nullptr)
        : options(options),
          defaultText(placeholderText),
          fontSize(fontSize),
          backgroundColor(backgroundColor),
          textColor(textColor),
          activeBackgroundColor(activeBackgroundColor),
          activeTextColor(activeTextColor),
          onSelectionChanged(std::move(onSelectionChanged)),
          text(0.0f, 0.0f, fontSize, defaultText),
          margin(0.0f),
          optionDisplayCount(optionDisplayCount),
          optionHeight(0.0f)
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->width = 0.0f;
        this->height = 0.0f;
        setRectSpec(rectSpec);
        setMarginSpec(marginSpec);
        initializeDropdownMenu();
        this->type = GUIObject;
    }

    void render() override
    {
        update();
        draw();
    }
    void update() override
    {}

    void resolveLayout(const sf::FloatRect& parentRect) override
    {
        RenderObject::resolveLayout(parentRect);
        initializeDropdownMenu();
    }
    void draw() override
    {
        window->draw(box);
        text.draw();
        if (arrowTexture != nullptr)
        {
            window->draw(arrowSprite);
        }

    }
    void hoverDraw() override
    {
        if (!active)
        {
            setHoverstate();
        }
        else
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
            auto bounds = std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, optionHeight));
            bool anyHovered = false;
            for (int i = 0; i < optionDisplayCount; i++)
            {
                bounds.first.y = y + height + optionHeight * i;
                if (GUIUtilities::positionInBounds(mousePosition, bounds))
                {
                    hoverIndex = i;
                    anyHovered = true;
                }
            }

            // Only let the mouse override keyboard selection when actually over an option row.
            if (anyHovered)
            {
                for (int i = 0; i < optionDisplayCount; i++)
                {
                    optionBoxes[i].setFillColor(i == hoverIndex ? activeBackgroundColor : backgroundColor);
                }
            }
        }
        draw();
    }
    void activeDraw() override
    {
        if (!active)
        {
            setActive();
        }
        draw();

        for (size_t i = 0; i < optionBoxes.size(); i++)
        {
            window->draw(optionBoxes[i]);
        }

        for (size_t i = 0; i < optionTexts.size(); i++)
        {
            optionTexts[i]->draw();
        }
    }


    void setHoverstate() override
    {
        this->hover = true;
        box.setFillColor(activeBackgroundColor);
        if (arrowTexture != nullptr)
        {
            arrowSprite.setColor(activeTextColor);
        }
    }
    void setUnhover() override
    {
        this->hover = false;
        box.setFillColor(backgroundColor);
        if (arrowTexture != nullptr)
        {
            arrowSprite.setColor(textColor);
        }
    }

    void setActive() override
    {
        textAtOpen = text.getString();
        selectedIndexAtOpen = selectedIndex;
        closeCommitsSelection = false;
        hoverIndex = -1;
        active = true;
        box.setSize(sf::Vector2f(width, height + (optionHeight * optionDisplayCount)));
        box.setFillColor(activeBackgroundColor);
        box.setOutlineColor(activeTextColor);
        text.setColor(activeTextColor);
        if (arrowTexture != nullptr)
        {
            arrowSprite.setColor(activeTextColor);
        }
    }
    void setInactive() override
    {
        if (active && !closeCommitsSelection)
        {
            text.setText(textAtOpen);
            selectedIndex = selectedIndexAtOpen;
        }
        closeCommitsSelection = false;
        hoverIndex = -1;
        active = false;
        box.setSize(sf::Vector2f(width, height));
        box.setFillColor(backgroundColor);
        box.setOutlineThickness(1);
        box.setOutlineColor(textColor);
        text.setColor(textColor);
        if (arrowTexture != nullptr)
        {
            arrowSprite.setColor(textColor);
        }
    }

    void onClick() override
    {
        // If we're not active yet, just open the dropdown; selection is handled on a second click.
        // When active, interpret the click position to pick an option.
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        auto bounds = std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, optionHeight));

        int clickedIndex = -1;
        for (int i = 0; i < static_cast<int>(optionDisplayCount); ++i)
        {
            bounds.first.y = y + height + optionHeight * i;
            if (GUIUtilities::positionInBounds(mousePosition, bounds))
            {
                clickedIndex = i;
                break;
            }
        }

        if (clickedIndex >= 0)
        {
            selectedIndex = clickedIndex;
            hoverIndex = -1;
            text.setText(optionTexts[selectedIndex]->getString());
            closeCommitsSelection = true;
            if (onSelectionChanged && selectedIndex >= 0 && selectedIndex < static_cast<int>(options.size()))
            {
                onSelectionChanged(this, options[static_cast<size_t>(selectedIndex)], selectedIndex);
            }
            this->setInactive();
        }
        // If no option row was hit, this was likely a click on the main box.
        // Do nothing here so the first click simply opens the dropdown
        // (RenderUtilities has already called setActive() for us).
    }

    void arrowKeyInput(sf::Keyboard::Key key) override
    {
        if (!active)
        {
            return;
        }

        const int optionCount = static_cast<int>(std::min(optionDisplayCount, options.size()));
        if (optionCount <= 0)
        {
            return;
        }

        switch (key)
        {
        case sf::Keyboard::Up:
        {
            if (hoverIndex < 0)
            {
                hoverIndex = (selectedIndex >= 0 && selectedIndex < optionCount) ? selectedIndex : 0;
            }
            hoverIndex = (hoverIndex - 1 + optionCount) % optionCount;
            updateKeyboardHighlight();
            break;
        }
        case sf::Keyboard::Down:
        {
            if (hoverIndex < 0)
            {
                hoverIndex = (selectedIndex >= 0 && selectedIndex < optionCount) ? selectedIndex : -1;
            }
            if (hoverIndex < 0)
            {
                hoverIndex = 0;
            }
            else
            {
                hoverIndex = (hoverIndex + 1) % optionCount;
            }
            updateKeyboardHighlight();
            break;
        }
        case sf::Keyboard::Return:
        {
            if (hoverIndex >= 0 && hoverIndex < optionCount)
            {
                selectedIndex = hoverIndex;
                hoverIndex = -1;
                text.setText(optionTexts[selectedIndex]->getString());
                closeCommitsSelection = true;
                if (onSelectionChanged && selectedIndex >= 0 && selectedIndex < static_cast<int>(options.size()))
                {
                    onSelectionChanged(this, options[static_cast<size_t>(selectedIndex)], selectedIndex);
                }
                this->setInactive();
            }
            break;
        }
        case sf::Keyboard::Escape:
        {
            // Cancel: close and restore previous selection/label via setInactive.
            this->setInactive();
            break;
        }
        default:
            break;
        }
    }

    void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        text.setWindow(window);
        for (size_t i = 0; i < optionTexts.size(); i++)
        {
            optionTexts[i]->setWindow(window);
        }
    }

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {

        if (!active)
            return std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, height));

        float totalHeight = height + (optionHeight * optionDisplayCount);
        return std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, totalHeight));

    }
    RenderObject& getHoverObject()
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        auto hoverArea = this->getHoverArea();

        if (GUIUtilities::positionInBounds(mousePosition, hoverArea))
        {
            return *this;
        }

        return *GUIUtilities::getEmptyRenderObject();
    }

    int getSelectedIndex() const
    {
        return selectedIndex;
    }

    bool hasSelection() const
    {
        return selectedIndex >= 0 && selectedIndex < static_cast<int>(options.size());
    }

    const T* getSelectedOptionPtr() const
    {
        if (!hasSelection())
        {
            return nullptr;
        }
        return &options[static_cast<size_t>(selectedIndex)];
    }


};
