#ifndef UI_H
#define UI_H

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

#endif