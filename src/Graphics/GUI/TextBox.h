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
    float textY, cursorY;
    float margin;

    int cursorIndex = 0; //the cursor index refers to the index where chars will add, 0 is the start
    float cursorPosition = -1;

    int selectionStart, selectionEnd;

    bool updateSelection = true;

    std::string textString;
    std::string oldString;

    sf::RectangleShape box;
    sf::RectangleShape selectionHighlight;
    sf::RectangleShape cursor;
    PrimitiveText text;
    PrimitiveText selectedText;

    std::chrono::steady_clock::time_point blinkTimeStart;
    float blinkPeriod = 500.0f;

    sf::Color backgroundColor, textColor, activeBackgroundColor, activeTextColor;


    template<typename T>
    T getDefaultValue() {
        return T();  // Default constructible types
    }
    template<>
    int getDefaultValue<int>() {
        return 0;
    }
    template<>
    float getDefaultValue<float>() {
        return 0.0f;
    }
    template<>
    std::string getDefaultValue<std::string>() {
        return "";
    }

    void initializeTextBox()
    {
        textY = y + height - (fontSize * 1.375f);
        cursorY = y + (height - fontSize) / 2;

        box = sf::RectangleShape(sf::Vector2f(width, height));
        selectionHighlight = sf::RectangleShape(sf::Vector2f((float)fontSize, height - (fontSize * 1.125f)));

        box.setPosition(sf::Vector2f(x, y));
        text.setText(textString);
        text.setPosition(sf::Vector2f(x + margin/2, textY));

        value = getDefaultValue<T>();
          

        setInactive();
    }
    void updateTextBox()
    {
        blinkTimeStart = std::chrono::steady_clock::now();

        //If the cursor is greater than what is displayed, place cursor at end of box and text accordingly
        if (cursorIndex > startIndex + displayLength)
        {
            cursorPosition = width - margin / 2 ;
            startIndex = 0;
            displayLength = cursorIndex;
            text.setText(textString.substr(startIndex, displayLength));
            if (text.getTextWidth() > width - margin)
            {
                while (text.getTextWidth() > width - margin && startIndex < cursorIndex)
                {
                    startIndex++;
                    displayLength--;
                    text.setText(textString.substr(startIndex, displayLength));
                }
                text.setAlign(ALIGN_RIGHT);
                text.setPosition(sf::Vector2f(x + cursorPosition, textY));
                cursor.setPosition(sf::Vector2f(x + cursorPosition, cursorY));
            }
            else
            {
                cursorPosition = text.getTextWidth() + margin / 2;
                text.setAlign(ALIGN_LEFT);
                text.setPosition(sf::Vector2f(x + margin / 2, textY));
                cursor.setPosition(sf::Vector2f(x + cursorPosition, cursorY));
            }
        }
        //If the cursor is leass than what is displayed, place cursors at start of box and text accordingly
        else if (cursorIndex < startIndex)
        {
            cursorPosition = margin / 2;
            startIndex = cursorIndex;
            text.setText(textString.substr(startIndex, displayLength));

            //remove chars from the end until the width is larger than the box
            while (text.getTextWidth() > width - margin && displayLength > 0)
            {
                displayLength--;
                text.setText(textString.substr(startIndex, displayLength));
            }
            text.setAlign(ALIGN_LEFT);
            text.setPosition(sf::Vector2f(x + cursorPosition, textY));
            cursor.setPosition(sf::Vector2f(x + cursorPosition, cursorY));
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
                    if (text.getTextWidth() >= width - margin)
                    {
                        cursorPosition = width - margin / 2;
                        while (text.getTextWidth() > width - margin && startIndex < cursorIndex)
                        {
                            startIndex++;
                            displayLength--;
                            text.setText(textString.substr(startIndex, displayLength));
                        }
                        text.setAlign(ALIGN_RIGHT);
                        text.setPosition(sf::Vector2f(x + cursorPosition, textY));
                        cursor.setPosition(sf::Vector2f(x + cursorPosition, cursorY));
                        return;
                    }
                }
            }
            
            
            //This is to ensure that the text doesn't jitter between cursor positions
            //Using this is more cosmetic, but can lead to slight discrepancies in text positioning
            /*float textX = text.getHoverArea().first.x;
            text.setPosition(text.getHoverArea().first);
            text.setAlign(ALIGN_LEFT);*/

            text.setPosition(sf::Vector2f(x + margin / 2, textY));
            text.setAlign(ALIGN_LEFT);

            text.setText(textString.substr(startIndex, (size_t)(cursorIndex - startIndex)));

            float widthToCursor = text.getTextWidth();
            cursor.setPosition(sf::Vector2f(x + margin / 2 + widthToCursor, cursorY));
            
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


        // Highlighting selected
        if (selectionStart == selectionEnd && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            selectionStart = -1;
            selectionEnd = -1;
        }

        if (selectionStart != -1)
        {
            int selectionMin = std::min(selectionStart, selectionEnd);
            int selectionMax = std::max(selectionStart, selectionEnd);

            
            selectionMin = std::max(selectionMin, startIndex);
            selectionMax = std::min(selectionMax, startIndex + displayLength);

            text.setText(textString.substr(startIndex, (size_t)(selectionMin - startIndex)));
            float preWidth = text.getTextWidth();
            text.setText(textString.substr(startIndex, (size_t)displayLength));
            preWidth += text.getHoverArea().first.x;

            selectedText.setText(textString.substr(selectionMin, (size_t)(selectionMax - selectionMin)));

            float selectionWidth = std::max(cursor.getPosition().x - preWidth, selectedText.getTextWidth());

            selectionHighlight.setPosition(sf::Vector2f(preWidth, cursorY));
            selectionHighlight.setSize(sf::Vector2f(selectionWidth, (float)fontSize * 1.25f));

            selectedText.setPosition(sf::Vector2f(preWidth, textY));

            selectionHighlight.setFillColor(activeTextColor);
            selectedText.setColor(activeBackgroundColor);
        }
        else
        {
            selectionHighlight.setFillColor(sf::Color::Transparent);
            selectedText.setColor(sf::Color::Transparent);
        }


    }
    void updateCursor()
    {
        auto blinkTime = blinkTimeStart - std::chrono::steady_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(blinkTime).count();

        if ((int)(milliseconds / blinkPeriod) % 2 == 0)
        {
            cursor.setFillColor(textColor);
        }
        else
        {
            cursor.setFillColor(sf::Color::Transparent);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (int)milliseconds % 20 == 0)
        {
            if (updateSelection)
            {
                updateSelection = false;
                auto mouse = sf::Mouse::getPosition(*window);
                getCursorIndex(mouse.x);
                selectionEnd = cursorIndex;
                if (selectionStart == -1)
                {
                    selectionStart = cursorIndex;
                }
                else
                {
                    updateTextBox();
                }
            }
        }
        else
        {
            updateSelection = true;
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

    void getCursorIndex(int mouseX)
    {
        //The minimum position for the cursor to be in to appear after a letter
        float cursorMinX = this->x + margin / 2;
        float difference = -1.0f;
        cursorIndex = startIndex;
        if (mouseX < x + margin / 2)
        {
            cursorIndex = std::max(startIndex - 1, 0);
        }

        while (mouseX > cursorMinX && cursorIndex < textString.size())
        {
            difference = std::abs(cursorMinX - mouseX);
            cursorIndex++;
            cursorMinX = this->x + margin / 2 + text.getTextWidth(textString.substr(startIndex, (size_t)(cursorIndex - startIndex)));
        }

        if (difference < std::abs(cursorMinX - mouseX) && difference != -1)
        {
            cursorIndex--;
        }
    }


public:
    TextBox(float x, float y, float width, float height, int size, std::string textString = "",
        sf::Color backgroundColor = sf::Color(200, 200, 200), sf::Color textColor = sf::Color(50, 50, 50),
        sf::Color activeBackgroundColor = sf::Color(255, 255, 255), sf::Color activeTextColor = sf::Color(0, 0, 0))
        :fontSize(size), textString(textString),
        backgroundColor(backgroundColor), textColor(textColor),
        activeBackgroundColor(activeBackgroundColor), activeTextColor(activeTextColor),
        text(x, y, size, textString), margin(height / 2), cursor(sf::Vector2f((float)size / 12, (float)size)),
        selectedText(x, y, size, ""),
        selectionStart(-1), selectionEnd(-1)

    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        initializeTextBox();
        this->type = GUIObject;
    }

    template<typename T>
    bool validateInput() {
        T newValue;
        bool success;
        std::istringstream iss(textString);

        iss >> newValue;
        success = !iss.fail() && (iss.eof() || iss.peek() == std::istringstream::traits_type::eof());
        if (success) {
            value = newValue;
        }
        return success;
    }
    template<>
    bool validateInput<int>() {
        int newValue;
        bool success;
        std::istringstream iss(textString);

        // First, extract as float to handle potential fractional part
        float floatValue;
        iss >> floatValue;

        // Check if the extraction from the stream was successful
        success = !iss.fail() && (iss.eof() || iss.peek() == std::istringstream::traits_type::eof());

        if (success) {
            // Round down using static_cast to int
            newValue = static_cast<int>(std::floor(floatValue));

            // Store the rounded value
            value = newValue;
        }

        return success;
    }
    template<>
    bool validateInput<float>() {
        bool success;
        std::istringstream iss(textString);

        // First, extract as float to handle potential fractional part
        float floatValue;
        iss >> floatValue;

        // Check if the extraction from the stream was successful
        success = !iss.fail() && (iss.eof() || iss.peek() == std::istringstream::traits_type::eof());

        if (success) {
            
            value = floatValue;
        }

        return success;
    }
    template<>
    bool validateInput<std::string>() {
        bool success;
        std::istringstream iss(textString);

        // First, extract as float to handle potential fractional part
        std::string newValue;

        std::getline(iss, newValue);

        // Check if the extraction from the stream was successful
        success = !iss.fail() && (iss.eof() || iss.peek() == std::istringstream::traits_type::eof());

        if (success) {

            value = newValue;
        }

        return success;
    }


    
    std::string valueToString() 
    {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    void setWindow(sf::RenderWindow* window)
    {
        this->window = window;
        this->text.setWindow(window);
        this->selectedText.setWindow(window);
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
    void hoverDraw() override 
    {
        setHoverstate();

        draw();
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
        window->draw(selectionHighlight);
        selectedText.draw();
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
        box.setFillColor(activeBackgroundColor);
        box.setOutlineColor(activeTextColor);
        text.setColor(activeTextColor);

        blinkTimeStart = std::chrono::steady_clock::now();
    }
    void setInactive() override
    {
        active = false;
        box.setFillColor(backgroundColor);
        box.setOutlineThickness(1);
        box.setOutlineColor(textColor);
        text.setColor(textColor);
        cursor.setFillColor(sf::Color::Transparent);
        selectionHighlight.setFillColor(sf::Color::Transparent);
        selectionHighlight.setOutlineThickness(0);

        validateInput<T>();

        textString = valueToString();

        truncateTextStart();


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
        /*sf::Cursor cursor;
        cursor.loadFromSystem(sf::Cursor::Text);

        window->setMouseCursor(cursor);*/

        auto mouse = sf::Mouse::getPosition(*window);
        //std::cout << "Textbox click\n";
        //std::cout << "x: " << mouse.x << "\t\ty: " << mouse.y << "\n";

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        {
            if (selectionStart == -1)
                selectionStart = cursorIndex;

            getCursorIndex(mouse.x);
            selectionEnd = cursorIndex;
        }
        else
        {
            getCursorIndex(mouse.x);
            selectionStart = -1;
            selectionEnd = -1;
        }

        updateTextBox();

        setActive();
    }
    void keyboardInput(sf::Uint32 input) override
    {
        if (input == '\b') { // Backspace
            // remove a character
            if (textString.size() > 0)
            {
                if (selectionStart != selectionEnd && selectionStart != -1)
                {
                    textString = textString.substr(0, std::min(selectionStart, selectionEnd)) + textString.substr(std::max(selectionStart, selectionEnd), textString.size() - std::max(selectionStart, selectionEnd));
                    cursorIndex = std::min(selectionStart, selectionEnd);

                    selectionStart = -1;
                    selectionEnd = -1;
                }
                else if (cursorIndex > 0)
                {
                    textString = textString.substr(0, cursorIndex - 1) + textString.substr(cursorIndex, textString.size() - cursorIndex);
                    cursorIndex = std::max(0, cursorIndex - 1);
                }
            }
        }
        else if (input < 32) //Control modified keys
        {
            switch (input)
            {
            case 1: //ctrl A
                selectionStart = 0;
                selectionEnd = (int)textString.size();
                cursorIndex = (int)textString.size() + 1;
                break;
            default:
                break;
            }
            //TODO ctrl
        }
        else
        {
            char newChar = static_cast<char>(input);

            if (selectionStart != selectionEnd && selectionStart != -1)
            {
                textString = textString.substr(0, std::min(selectionStart, selectionEnd)) + textString.substr(std::max(selectionStart, selectionEnd), textString.size() - std::max(selectionStart, selectionEnd));
                cursorIndex = std::min(selectionStart, selectionEnd);

                selectionStart = -1;
                selectionEnd = -1;
            }

            

            textString = textString.substr(0, cursorIndex) + newChar + textString.substr(cursorIndex, textString.size() - cursorIndex);
            cursorIndex++;
        }
        updateTextBox();
        //std::cout << textString << std::endl;
    }
    void arrowKeyInput(sf::Keyboard::Key key) override 
    {
        switch (key)
        {
        case sf::Keyboard::Up:
            break;
        case sf::Keyboard::Down:
            break;
        case sf::Keyboard::Left:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RShift))
            {
                if (selectionStart == -1)
                {
                    selectionStart = cursorIndex;
                    cursorIndex = std::max(cursorIndex - 1, 0);
                    selectionEnd = cursorIndex;
                }
                else
                {
                    cursorIndex = std::max(cursorIndex - 1, 0);
                    selectionEnd = cursorIndex;
                }
                //std::cout << "Start: " << selectionStart << "\t\tEnd:" << selectionEnd << std::endl;
            }
            else
            {
                if (selectionStart != -1)
                {
                    cursorIndex = std::min(selectionStart, selectionEnd);

                    selectionStart = -1;
                    selectionEnd = -1;
                }
                else
                {
                    cursorIndex = std::max(cursorIndex - 1, 0);
                }
            }
            break;
        case sf::Keyboard::Right:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RShift))
            {
                if (selectionStart == -1)
                {
                    selectionStart = cursorIndex;
                    cursorIndex = std::min(cursorIndex + 1, (int)textString.size());
                    selectionEnd = cursorIndex;
                }
                else
                {
                    cursorIndex = std::min(cursorIndex + 1, (int)textString.size());
                    selectionEnd = cursorIndex;
                }

                //std::cout << "Start: " << selectionStart << "\t\tEnd:" << selectionEnd << std::endl;
            }
            else
            {
                if (selectionStart != -1)
                {
                    cursorIndex = std::max(selectionStart, selectionEnd);

                    selectionStart = -1;
                    selectionEnd = -1;
                }
                else
                {
                    cursorIndex = std::min(cursorIndex + 1, (int)textString.size());
                }
            }
            break;
        case sf::Keyboard::Delete:
            if (cursorIndex < textString.size())
            textString = textString.substr(0, (size_t)cursorIndex) + textString.substr(cursorIndex + 1, textString.size() - (cursorIndex + 1));
            //std::cout << textString << std::endl;
            break;
        case::sf::Keyboard::Home:
            cursorIndex = 0;
            break;
        case::sf::Keyboard::End:
            cursorIndex = (int)textString.size();
            break;
        default:
            break;
        }

        blinkTimeStart = std::chrono::steady_clock::now();
        updateTextBox();
    }

    void setValue(T newValue) {
        value = newValue;
    }

};

template<>
void TextBox<int>::setValue(int newValue)
{
    this->value = newValue;
    this->textString = valueToString();

    updateTextBox();
}
template<>
void TextBox<float>::setValue(float newValue)
{
    this->value = newValue;
    this->textString = valueToString();

    updateTextBox();
}

template<>
void TextBox<std::string>::setValue(std::string newValue)
{
    this->textString = newValue;
    this->validateInput<std::string>();
    this->updateTextBox();
}
