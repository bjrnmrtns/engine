#include "UiElements.h"

std::vector<UI::Vertex> generateSquare(int x, int y, int w, int h)
{
    std::vector<UI::Vertex> buffer;
    buffer.push_back(UI::Vertex(glm::vec2(x, y), glm::vec3(0.0f, 0.0f, 0.0f)));
    buffer.push_back(UI::Vertex(glm::vec2(x, y + h), glm::vec3(0.0f, 0.0f, 0.0f)));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), glm::vec3(0.0f, 0.0f, 0.0f)));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y), glm::vec3(0.0f, 0.0f, 0.0f)));
    buffer.push_back(UI::Vertex(glm::vec2(x, y), glm::vec3(0.0f, 0.0f, 0.0f)));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), glm::vec3(0.0f, 0.0f, 0.0f)));
    return buffer;
}

std::vector<UI::Vertex> SelectionRectangle(glm::vec2 pointA, glm::vec2 pointB)
{
    int xlowerleft = std::min(pointA.x, pointB.x);
    int ylowerleft = std::min(pointA.y, pointB.y);
    int w = std::abs(pointA.x - pointB.x);
    int h = std::abs(pointA.y - pointB.y);
    int thickness = 2;
    std::vector<UI::Vertex> leftbar(generateSquare(xlowerleft, ylowerleft, thickness, h));
    std::vector<UI::Vertex> rightbar(generateSquare(xlowerleft + w, ylowerleft, thickness, h + thickness));
    std::vector<UI::Vertex> lowbar(generateSquare(xlowerleft, ylowerleft, w, thickness));
    std::vector<UI::Vertex> highbar(generateSquare(xlowerleft, ylowerleft + h, w, thickness));
    std::vector<UI::Vertex> buffer(leftbar);
    buffer.insert(buffer.end(), rightbar.begin(), rightbar.end());
    buffer.insert(buffer.end(), lowbar.begin(), lowbar.end());
    buffer.insert(buffer.end(), highbar.begin(), highbar.end());
    return buffer;
}

