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
    int startIndex, cursorIndex = 0; //the cursor index refers to the index where chars will add, 0 is the start

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
        text.setPosition(sf::Vector2f(x, y + height - fontSize * 1.5));

        setInactive();
    }
    void truncateTextEnd()
    {
        text.setText(textString.substr(startIndex, textString.size() - startIndex));
        while (text.getHoverArea().second.x < this->width && startIndex != 0)
        {
            startIndex--;
            text.setText(textString.substr(startIndex, textString.size() - startIndex));
        }
        while (text.getHoverArea().second.x > this->width && startIndex < textString.size())
        {
            startIndex++;
            text.setText(textString.substr(startIndex, textString.size() - startIndex));
        }
    }
    void truncateTextStart()
    {
        startIndex = 0;
        while (text.getHoverArea().second.x < this->width && startIndex < textString.size())
        {
            startIndex++;
            text.setText(textString.substr(0, startIndex));
        }
        while (text.getHoverArea().second.x > this->width && startIndex != 0)
        {
            startIndex--;
            text.setText(textString.substr(0, startIndex));
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
        truncateTextStart();
    }
public:
    TextBox(float x, float y, float width, float height, int size, std::string textString = "",
        sf::Color backgroundColor = sf::Color(200, 200, 200), sf::Color textColor = sf::Color(50, 50, 50), 
        sf::Color activeBackgroundColor = sf::Color(255, 255, 255), sf::Color activeTextColor = sf::Color(0, 0, 0))
        :fontSize(size), textString(textString),
        backgroundColor(backgroundColor), textColor(textColor),
        activeBackgroundColor(activeBackgroundColor), activeTextColor(activeTextColor),
        text(x, y, size, textString)

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
    void update() override {}
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
        auto blinkTime = blinkTimeStart - std::chrono::steady_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(blinkTime).count();

        if ((int)(milliseconds / blinkPeriod) % 2 == 0)
        {
            cursor.setFillColor(sf::Color::Transparent);
        }
        else
        {
            cursor.setFillColor(activeTextColor);
        }
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
            }
        }
        else if (input < 32) //Control modified keys
        {
            //TODO ctrl
        }
        else
        {
            textString += static_cast<char>(input);
        }


        text.setText(textString);
        truncateTextEnd();

        std::cout << textString << std::endl;

    }
};

