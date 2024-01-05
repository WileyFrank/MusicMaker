#pragma once
#include "GUIUtilities.h"
#include "RenderObject.h"
#include "Primitives/PrimitiveText.h"


template <typename T>
class TextBox :
    public RenderObject
{
private:

    T value; //The value of the string as type T

    int fontSize, textDisplayStart = 0;
    int startIndex, displayLength;
    float margin;

    int cursorIndex = 0; //the cursor index refers to the index where chars will add, 0 is the start
    float cursorPosition = -1;

    std::string textString;

    sf::RectangleShape box;
    sf::RectangleShape cursor;
    PrimitiveText text;

    std::chrono::steady_clock::time_point blinkTimeStart;
    float blinkPeriod = 500.0f;

    sf::Color backgroundColor, textColor, activeBackgroundColor, activeTextColor;

    void initializeTextBox()
    {
        box = sf::RectangleShape(sf::Vector2f(width, height));
        box.setPosition(sf::Vector2f(x, y));
        text.setText(textString);
        text.setPosition(sf::Vector2f(x, y + height - (fontSize * 1.25f)));

        setInactive();
    }
    void updateTextBox()
    {
        //If the cursor is greater than what is displayed, place cursor at end of box and text accordingly
        if (cursorIndex > startIndex + displayLength)
        {
            cursorPosition = width - margin / 2;
            startIndex = 0;
            displayLength = cursorIndex;
            text.setText(textString.substr(startIndex, displayLength));
            if (text.getHoverArea().second.x > width - margin)
            {
                while (text.getHoverArea().second.x > width - margin && startIndex < cursorIndex)
                {
                    startIndex++;
                    displayLength--;
                    text.setText(textString.substr(startIndex, displayLength));
                }
                text.setAlign(ALIGN_RIGHT);
                text.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.25f)));
                cursor.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.125f)));
            }
            else
            {
                cursorPosition = text.getHoverArea().second.x + margin / 2;
                text.setAlign(ALIGN_LEFT);
                text.setPosition(sf::Vector2f(x + margin / 2, y + height - (fontSize * 1.25f)));
                cursor.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.125f)));
            }
        }
        //If the cursor is leass than what is displayed, place cursors at start of box and text accordingly
        else if (cursorIndex < startIndex)
        {
            cursorPosition = margin / 2;
            startIndex = cursorIndex;
            text.setText(textString.substr(startIndex, displayLength));

            //remove chars from the end until the width is larger than the box
            while (text.getHoverArea().second.x > width - margin && displayLength > 0)
            {
                displayLength--;
                text.setText(textString.substr(startIndex, displayLength));
            }
            text.setAlign(ALIGN_LEFT);
            text.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.25f)));
            cursor.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.125f)));
        }
        //the cursor is in the bounds of the box
        //this includes when the last charachter is deleted
        else 
        {
            

            //On backspace
            if (displayLength > textString.size() - startIndex)
            {
                displayLength = (int)textString.size() - startIndex;

                if (cursorIndex == textString.size())
                {
                    // if the cursor is at the end deleting from a string in which not all is visible
                    text.setText(textString);
                    startIndex = 0;
                    displayLength = cursorIndex;
                    if (text.getHoverArea().second.x >= width - margin)
                    {
                        cursorPosition = width - margin / 2;
                        while (text.getHoverArea().second.x > width - margin && startIndex < cursorIndex)
                        {
                            startIndex++;
                            displayLength--;
                            text.setText(textString.substr(startIndex, displayLength));
                        }
                        text.setAlign(ALIGN_RIGHT);
                        text.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.25f)));
                        cursor.setPosition(sf::Vector2f(x + cursorPosition, y + height - (fontSize * 1.125f)));
                        return;
                    }
                }
            }
            
            
            //This is to ensure that the text doesn't jitter between cursor positions
            //Using this is more cosmetic, but can lead to slight discrepancies in text positioning
            /*float textX = text.getHoverArea().first.x;
            text.setPosition(text.getHoverArea().first);
            text.setAlign(ALIGN_LEFT);*/

            text.setPosition(sf::Vector2f(x + margin / 2, y + height - (fontSize * 1.25f)));
            text.setAlign(ALIGN_LEFT);

            text.setText(textString.substr(startIndex, (size_t)(cursorIndex - startIndex)));

            float widthToCursor = text.getHoverArea().second.x;
            cursor.setPosition(sf::Vector2f(x + margin / 2 + widthToCursor, y + height - (fontSize * 1.125f)));
            
            text.setText(textString.substr(startIndex, displayLength));

            while (text.getHoverArea().second.x < width - margin && displayLength < textString.size() - startIndex)
            {
                displayLength++;
                text.setText(textString.substr(startIndex, displayLength));
            }

            while (text.getHoverArea().second.x > width - margin && displayLength > 0)
            {
                displayLength--;
                text.setText(textString.substr(startIndex, displayLength));
            }

        }

    }
    void updateCursor()
    {
        auto blinkTime = blinkTimeStart - std::chrono::steady_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(blinkTime).count();

        if ((int)(milliseconds / blinkPeriod) % 2 == 0)
        {
            cursor.setFillColor(sf::Color::Transparent);
        }
        else
        {
            cursor.setFillColor(textColor);
        }

    }

    void truncateTextIndex(int index)
    {
        startIndex = std::min((int)textString.size(), index);
        startIndex = std::max(0, startIndex);
        displayLength = std::max(std::min((int)textString.size() - index, displayLength), 0);
        text.setText(textString.substr(startIndex, displayLength));
        while (text.getHoverArea().second.x < this->width - margin && displayLength <= textString.size() - startIndex)
        {
            displayLength++;
            text.setText(textString.substr(startIndex, displayLength));
        }
        while (text.getHoverArea().second.x > this->width - margin && displayLength > 0)
        {
            displayLength--;
            text.setText(textString.substr(startIndex, displayLength));
        }
    }
    void truncateTextCursor()
    {
        truncateTextIndex(startIndex);




        if (cursorIndex < startIndex + 2)
        {
            //start is the index of the cursor within bounds of the text
            startIndex = std::max((std::min((int)textString.size(), cursorIndex - 2)), 0);
            text.setText(textString.substr(startIndex, displayLength));
            while (text.getHoverArea().second.x > this->width - margin && displayLength != 0)
            {
                displayLength--;
                text.setText(textString.substr(startIndex, displayLength));
            }
            return;
        }
        else if (cursorIndex > startIndex + displayLength)
        {
            displayLength = cursorIndex - startIndex;
            text.setText(textString.substr(startIndex, displayLength));
            while (text.getHoverArea().second.x > this->width - margin)
            {
                startIndex++;
                displayLength = cursorIndex - startIndex;
                text.setText(textString.substr(startIndex, displayLength));
            }
            return;
        }
        else
        {
            return;
        }
    }
    void truncateTextEnd()
    {
        text.setText(textString.substr(startIndex, textString.size() - startIndex));
        while (text.getHoverArea().second.x < this->width - margin && startIndex != 0)
        {
            startIndex--;
            text.setText(textString.substr(startIndex, textString.size() - startIndex));
        }
        while (text.getHoverArea().second.x > this->width - margin && startIndex < textString.size())
        {
            startIndex++;
            text.setText(textString.substr(startIndex, textString.size() - startIndex));
        }
        displayLength = (int)textString.size() - startIndex;
    }
    void truncateTextStart()
    {
        startIndex = 0;
        displayLength = 0;
        while (text.getHoverArea().second.x < this->width - margin && displayLength < textString.size())
        {
            displayLength++;
            text.setText(textString.substr(0, displayLength));
        }
        while (text.getHoverArea().second.x > this->width - margin && displayLength != 0)
        {
            displayLength--;
            text.setText(textString.substr(0, displayLength));
        }
    }

    void setActive() override
    {
        active = true;
        box.setFillColor(activeBackgroundColor);
        box.setOutlineColor(activeTextColor);
        box.setOutlineThickness(3);
        text.setColor(activeTextColor);

        blinkTimeStart = std::chrono::steady_clock::now();
    }

    void setInactive() override
    {
        active = false;
        box.setFillColor(backgroundColor);
        box.setOutlineThickness(2);
        box.setOutlineColor(textColor);
        text.setColor(textColor);
        cursor.setFillColor(sf::Color::Transparent);
        truncateTextStart();
    }
public:
    TextBox(float x, float y, float width, float height, int size, std::string textString = "",
        sf::Color backgroundColor = sf::Color(200, 200, 200), sf::Color textColor = sf::Color(50, 50, 50),
        sf::Color activeBackgroundColor = sf::Color(255, 255, 255), sf::Color activeTextColor = sf::Color(0, 0, 0))
        :fontSize(size), textString(textString),
        backgroundColor(backgroundColor), textColor(textColor),
        activeBackgroundColor(activeBackgroundColor), activeTextColor(activeTextColor),
        text(x, y, size, textString), margin(height / 2), cursor(sf::Vector2f((float)size / 8, (float)size))

    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        initializeTextBox();
        this->type = GUIObject;
    }
    
    bool ValidateInput()
    {
        // Check if the input string can be converted to type T
        T value;
        std::istringstream iss(textString);
        iss >> value;
        return !(iss.fail() || !iss.eof());
    }

    void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        this->text.setWindow(window);
    }

    void render() override
    {
        update();
        draw();
    }
    void update() override 
    {
        if (active)
        {
            updateCursor();
        }
    }
    void draw() override
    {
        window->draw(box);
        text.draw();
    }
    void activeDraw() override 
    {
        if (!active)
        {
            setActive();
        }
        updateCursor();
        draw();
        window->draw(cursor);
    }

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override {
        return std::make_pair(sf::Vector2f(x, y), sf::Vector2f(width, height));
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

    void onClick() override
    {
        std::cout << "Textbox click\n";
        setActive();
    }
    void keyboardInput(sf::Uint32 input) override
    {
        if (input == '\b') { // Backspace
            // remove a character
            if (textString.size() > 0)
            {
                textString = textString.substr(0, textString.size() - 1);
                cursorIndex = std::max(0, cursorIndex - 1);

            }
        }
        else if (input < 32) //Control modified keys
        {
            //TODO ctrl
        }
        else
        {
            textString += static_cast<char>(input);
            cursorIndex++;
        }

        updateTextBox();

        std::cout << textString << std::endl;

    }
};

