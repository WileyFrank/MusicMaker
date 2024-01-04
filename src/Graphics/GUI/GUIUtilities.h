#pragma once
#include <mutex>
#include "NullRenderObject.h"


class GUIUtilities
{
private:

    GUIUtilities();

   static std::unique_ptr<GUIUtilities> instance;

   static std::mutex mutex;

   static NullRenderObject empty;


public:

    static GUIUtilities* GetInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance = std::unique_ptr<GUIUtilities>(new GUIUtilities());
        }
        return instance.get();
    }
    
    static RenderObject* getEmptyRenderObject()
    {
        return &empty;
    }

    static bool positionInBounds(sf::Vector2f position, std::pair<sf::Vector2f, sf::Vector2f> bounds)
    {
        if (position.x > bounds.first.x && position.x < bounds.first.x + bounds.second.x)
        {
            if (position.y > bounds.first.y && position.y < bounds.first.y + bounds.second.y)
            {
                return true;
            }
        }
        return false;
    }
    
    static bool positionInBounds(sf::Vector2i position, std::pair<sf::Vector2f, sf::Vector2f> bounds)
    {
        if (position.x > bounds.first.x && position.x < bounds.first.x + bounds.second.x)
        {
            if (position.y > bounds.first.y && position.y < bounds.first.y + bounds.second.y)
            {
                return true;
            }
        }
        return false;
    }

    //Shape specific
    static sf::ConvexShape createRoundedRectangle(float width, float height, float cornerRadius, int cornerPointCount) {
        sf::ConvexShape shape((unsigned)(4 * cornerPointCount + 8)); // Four corners, each with cornerPointCount points, plus 4 for the sides
        std::size_t pointIndex = 0;

        // Helper lambda to set points in the correct order
        auto setPoint = [&](float x, float y) {
            shape.setPoint(pointIndex++, sf::Vector2f(x, y));
            };

        // Top left corner
        std::vector<std::pair<float , float>> points;
        std::vector<std::pair<float , float>> anchorPoints;
        points.push_back(std::make_pair(width - cornerRadius, cornerRadius));
        points.push_back(std::make_pair(cornerRadius, cornerRadius));
        points.push_back(std::make_pair(cornerRadius, height - cornerRadius));
        points.push_back(std::make_pair(width - cornerRadius, height - cornerRadius));

        anchorPoints.push_back(std::make_pair(width, cornerRadius));
        anchorPoints.push_back(std::make_pair(cornerRadius, 0.0f));
        anchorPoints.push_back(std::make_pair(0.0f, height-cornerRadius));
        anchorPoints.push_back(std::make_pair(width - cornerRadius, height));

        anchorPoints.push_back(std::make_pair(width - cornerRadius, 0.0f));
        anchorPoints.push_back(std::make_pair(0.0f, cornerRadius));
        anchorPoints.push_back(std::make_pair(cornerRadius, height));
        anchorPoints.push_back(std::make_pair(width, height-cornerRadius));


        float pi = 3.14159265f;

        for (int i = 0; i < 4; i++)
        {
            setPoint(anchorPoints[i].first, anchorPoints[i].second);
            for (int j = 0; j < (float)cornerPointCount; j++)
            {
                float angle = ((90.0f * i) + (j * (90.0f / cornerPointCount))) * pi / 180.0f;
                float x = points[i].first + cos(angle) * cornerRadius;
                float y = points[i].second - sin(angle) * cornerRadius;

                setPoint(x, y);
            }
            setPoint(anchorPoints[(size_t)(i + 4)].first, anchorPoints[(size_t)(i + 4)].second);
        }

        // Complete the shape by setting the last point equal to the first
        shape.setPoint(pointIndex-1, shape.getPoint(0));

        return shape;
    }

};


