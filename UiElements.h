#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "VertexDefinitions.h"
#include <vector>

std::vector<UI::Vertex> generateSquare(int x, int y, int w, int h);
std::vector<UI::Vertex> SelectionRectangle(glm::vec2 pointA, glm::vec2 pointB);

#endif
