#ifndef FONT_H
#define FONT_H

#include <glm/glm.hpp>
#include <vector>

struct textvertex
{
    textvertex(glm::vec2 pos, glm::vec3 color, glm::vec2 texcoord)
    : pos(pos)
    , color(color)
    , texcoord(texcoord)
    {
    }
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texcoord;
};

bool GenerateText(const char text[], std::vector<textvertex>& textbuf);

#endif
