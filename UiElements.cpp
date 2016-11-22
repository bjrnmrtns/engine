#include "UiElements.h"
#include "VertexDefinitions.h"
#include <vector>

void generateSquare(std::vector<UI::Vertex>& buffer, int x, int y, int w, int h, glm::vec3 color)
{
    buffer.push_back(UI::Vertex(glm::vec2(x - w, y - h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x - w, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y - h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x - w, y - h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), color));
}

