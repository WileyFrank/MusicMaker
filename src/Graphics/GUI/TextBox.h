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
        if (cursorPosition < 0)
        {
            //sets start to 0 and length to max in width
            truncateTextStart();

            cursorPosition = text.getHoverArea().second.x;
        }

        if (cursorPosition > width - margin)
        {
            cursorPosition = width - margin / 2;
            text.setAlign(ALIGN_RIGHT);

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

        float widthToCursor = 0.0f;
        if (cursorIndex - startIndex > 0)
        {
            text.setText(textString.substr(startIndex, (cursorIndex - startIndex)));
            widthToCursor = text.getHoverArea().second.x;
            text.setText(textString.substr(startIndex, displayLength));
        }

        cursor.setPosition(sf::Vector2f(x + widthToCursor, y + height - (fontSize * 1.125f)));

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

        text.setText(textString);
        truncateTextCursor();

        std::cout << textString << std::endl;

    }
};

