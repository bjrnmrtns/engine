#ifndef UI_H
#define UI_H

#include "VertexDefinitions.h"
#include <vector>

struct UIState
{
    UIState(int mousex, int mousey);
    bool regionhit(int x, int y, int w, int h);
    int mousex;
    int mousey;
    int mousedown;
    int hotitem;
    int activeitem;
};

void generateSquare(std::vector<UI::Vertex>& buffer, int x, int y, int w, int h, glm::vec3 color);

#endif
