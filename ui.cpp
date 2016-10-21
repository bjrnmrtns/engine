#include "ui.h"

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