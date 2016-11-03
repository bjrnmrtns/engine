#include "ui.h"
#include "VertexDefinitions.h"
#include <vector>

UIState::UIState(int mousex, int mousey)
: mousex(mousex)
, mousey(mousey)
, mousedown(false)
, hotitem(0)
, activeitem(0)
{
}

bool UIState::regionhit(int x, int y, int w, int h)
{
    return !(mousex < x || mousey < y ||
           mousex >= x + w || mousey >= y + h);
}

void generateSquare(std::vector<UI::Vertex>& buffer, int x, int y, int w, int h, glm::vec3 color)
{
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x, y), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y + h), color));
    buffer.push_back(UI::Vertex(glm::vec2(x, y), color));
    buffer.push_back(UI::Vertex(glm::vec2(x + w, y), color));
}

