#pragma once
#include "GUIUtilities.h"
#include "RenderObject.h"
#include "Primitives/PrimitiveText.h"

template <typename T>
class DropdownMenu : public RenderObject {

private:

    int  fontSize;
    float optionHeight, margin, textY;

    std::string defaultText = "Select an Option";
    std::vector<T> options;
    int selectedIndex = -1;
    int hoverIndex = -1;
    bool isExpanded = false; // To track if the dropdown is expanded
    size_t optionDisplayCount; // The amount of options to display when expanded
    size_t startDisplayIndex;
    sf::RectangleShape box; // Main dropdown box
    PrimitiveText text;
    std::vector<sf::RectangleShape> optionBoxes; // Shapes for each option
    std::vector<PrimitiveText*> optionTexts; // Text for each option
    sf::Color backgroundColor, textColor, activeBackgroundColor, activeTextColor;

    void initializeDropdownMenu() {
        // Main dropdown appearance
        box.setSize(sf::Vector2f(width, height));
        box.setPosition(sf::Vector2f(x, y));
        box.setFillColor(backgroundColor);

        textY = y + height - (fontSize * 1.375f);
        text.setText(defaultText);
        text.setPosition(sf::Vector2f(x + margin / 2, textY));

        // Initialize each option box and text based on the options
        optionBoxes.clear();
        optionTexts.clear();
        float boxY;
        std::string optionString = "---";
        for (size_t i = 0; i < optionDisplayCount; ++i) {
            boxY = y + height + optionHeight * (i);
            sf::RectangleShape optionBox(sf::Vector2f(width, height));
            
            optionBox.setPosition(sf::Vector2f(x, boxY));
            optionBox.setFillColor(backgroundColor);
            optionBoxes.push_back(optionBox);
            textY = boxY + optionHeight - (fontSize * 1.375f);
            PrimitiveText* optionText = new PrimitiveText(x, textY, fontSize, optionString);
            optionText->setText("----");
            optionText->setColor(textColor);
            optionText->setPosition(sf::Vector2f(x + margin / 2, textY));
            optionTexts.push_back(optionText);
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
        sf::Color activeBackgroundColor = sf::Color(255, 255, 255), sf::Color activeTextColor = sf::Color(0, 0, 0))
        :options(options), fontSize(fontSize), backgroundColor(backgroundColor), textColor(textColor),
        activeBackgroundColor(activeBackgroundColor), activeTextColor(activeTextColor), text(x, y, fontSize, defaultText), 
        margin(height/2), optionDisplayCount(optionDisplayCount), optionHeight(height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
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
    void draw() override
    {
        window->draw(box);
        text.draw();

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
            selectedIndex = -1;
            auto bounds = std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, optionHeight));
            hoverIndex = -1;
            for (int i = 0; i < optionDisplayCount; i++)
            {
                bounds.first.y = y + height + optionHeight * i;
                if (GUIUtilities::positionInBounds(mousePosition, bounds))
                {
                    hoverIndex = i;
                    optionBoxes[hoverIndex].setFillColor(activeBackgroundColor);
                }
                else
                {
                    optionBoxes[i].setFillColor(backgroundColor);
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
    }
    void setUnhover() override
    {
        this->hover = false;
        box.setFillColor(backgroundColor);
    }

    void setActive() override
    {
        active = true;
        box.setSize(sf::Vector2f(width, height + (optionHeight * optionDisplayCount)));
        box.setFillColor(activeBackgroundColor);
        box.setOutlineColor(activeTextColor);
        text.setColor(activeTextColor);
    }
    void setInactive() override
    {
        active = false;
        box.setSize(sf::Vector2f(width, height));
        box.setFillColor(backgroundColor);
        box.setOutlineThickness(1);
        box.setOutlineColor(textColor);
        text.setColor(textColor);
    }

    void onClick() override
    {
        if (hoverIndex >= 0)
        {
            selectedIndex = hoverIndex;
            hoverIndex = -1;
            text.setText(optionTexts[selectedIndex]->getString());
            this->setInactive();
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


};
