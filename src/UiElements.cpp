#include "UiElements.hpp"

std::vector<UI::Vertex> generateSquare(int x, int y, int w, int h, const glm::vec3 &color) {
    std::vector<UI::Vertex> buffer;
    buffer.push_back(UI::Vertex(glm::vec2(x, y), color));
    buffer.push_back(UI::Vertex(glm::vec2(x, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y), color));
    buffer.push_back(UI::Vertex(glm::vec2(x, y), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), color));
    return buffer;
}

std::vector<UI::Vertex> SelectionRectangle(const glm::vec2 &pointA, const glm::vec2 &pointB, const glm::vec3 &color) {
    int xlowerleft = std::min(pointA.x, pointB.x);
    int ylowerleft = std::min(pointA.y, pointB.y);
    int w = std::abs(pointA.x - pointB.x);
    int h = std::abs(pointA.y - pointB.y);
    int thickness = 2;
    std::vector<UI::Vertex> leftbar(generateSquare(xlowerleft, ylowerleft, thickness, h, color));
    std::vector<UI::Vertex> rightbar(generateSquare(xlowerleft + w, ylowerleft, thickness, h + thickness, color));
    std::vector<UI::Vertex> lowbar(generateSquare(xlowerleft, ylowerleft, w, thickness, color));
    std::vector<UI::Vertex> highbar(generateSquare(xlowerleft, ylowerleft + h, w, thickness, color));
    std::vector<UI::Vertex> buffer(leftbar);
    buffer.insert(buffer.end(), rightbar.begin(), rightbar.end());
    buffer.insert(buffer.end(), lowbar.begin(), lowbar.end());
    buffer.insert(buffer.end(), highbar.begin(), highbar.end());
    return buffer;
}
