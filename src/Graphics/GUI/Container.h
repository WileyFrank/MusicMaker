#pragma once

#include <algorithm>
#include <vector>

#include "GUIUtilities.h"
#include "RenderObject.h"

class Container : public RenderObject
{
private:
    std::vector<RenderObject*> children;
    float paddingLeft = 0.0f;
    float paddingTop = 0.0f;
    float paddingRight = 0.0f;
    float paddingBottom = 0.0f;
    sf::FloatRect contentRect;

    void updateContentRectFromResolvedRect()
    {
        const sf::FloatRect outer = getResolvedRect();
        const float contentLeft = outer.left + paddingLeft;
        const float contentTop = outer.top + paddingTop;
        const float contentWidth = std::max(0.0f, outer.width - paddingLeft - paddingRight);
        const float contentHeight = std::max(0.0f, outer.height - paddingTop - paddingBottom);

        contentRect = sf::FloatRect(contentLeft, contentTop, contentWidth, contentHeight);
    }

public:
    Container()
        : RenderObject(0.0f, 0.0f, 0.0f, 0.0f)
    {
        type = GUIObject;
        updateContentRectFromResolvedRect();
    }

    Container(float x, float y, float width, float height)
        : RenderObject(x, y, width, height)
    {
        type = GUIObject;
        updateContentRectFromResolvedRect();
    }

    void addChild(RenderObject* child)
    {
        if (child == nullptr) {
            return;
        }

        if (window != nullptr) {
            child->setWindow(window);
        }

        children.push_back(child);

        // If this container is already resolved, place the new child immediately.
        child->resolveLayout(getChildLayoutRect());
    }

    std::vector<RenderObject*>& getChildren()
    {
        return children;
    }

    const std::vector<RenderObject*>& getChildren() const
    {
        return children;
    }

    void clearChildren()
    {
        children.clear();
    }

    void setPadding(float all)
    {
        setPadding(all, all, all, all);
    }

    void setPadding(float left, float top, float right, float bottom)
    {
        paddingLeft = left;
        paddingTop = top;
        paddingRight = right;
        paddingBottom = bottom;
        updateContentRectFromResolvedRect();
    }

    sf::FloatRect getContentRect() const
    {
        return contentRect;
    }

    void setWindow(sf::RenderWindow* window) override
    {
        this->window = window;

        for (auto* child : children) {
            if (child != nullptr) {
                child->setWindow(window);
            }
        }
    }

    virtual sf::FloatRect getChildLayoutRect() const
    {
        return contentRect;
    }

    void resolveLayout(const sf::FloatRect& parentRect) override
    {
        RenderObject::resolveLayout(parentRect);
        updateContentRectFromResolvedRect();
        resolveChildrenLayout();
    }

    virtual void resolveChildrenLayout()
    {
        const sf::FloatRect childParentRect = getChildLayoutRect();

        for (auto* child : children) {
            if (child != nullptr) {
                child->resolveLayout(childParentRect);
            }
        }
    }

    void resolveFromWindow()
    {
        if (window == nullptr) {
            return;
        }

        const sf::Vector2u windowSize = window->getSize();
        setResolvedRect(sf::FloatRect(0.0f, 0.0f, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
        updateContentRectFromResolvedRect();
        resolveChildrenLayout();
    }

    virtual void renderChildren()
    {
        for (auto* child : children) {
            if (child != nullptr) {
                child->render();
            }
        }
    }

    virtual void updateChildren()
    {
        for (auto* child : children) {
            if (child != nullptr) {
                child->update();
            }
        }
    }

    virtual void drawChildren()
    {
        for (auto* child : children) {
            if (child != nullptr) {
                child->draw();
            }
        }
    }

    void render() override
    {
        renderChildren();
    }

    void update() override
    {
        updateChildren();
    }

    void draw() override
    {
        drawChildren();
    }

    std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override
    {
        const sf::FloatRect pixelRect = getPixelRect();
        return std::make_pair(
            sf::Vector2f(pixelRect.left, pixelRect.top),
            sf::Vector2f(pixelRect.width, pixelRect.height)
        );
    }

    RenderObject& getHoverObject() override
    {
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if (*it == nullptr) {
                continue;
            }

            auto& hoverObject = (*it)->getHoverObject();
            if (hoverObject.getType() != EmptyRenderObject) {
                return hoverObject;
            }
        }

        if (window != nullptr && GUIUtilities::positionInBounds(sf::Mouse::getPosition(*window), getHoverArea())) {
            return *this;
        }

        return *GUIUtilities::getEmptyRenderObject();
    }
};
