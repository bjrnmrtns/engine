#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "VertexDefinitions.h"
#include <vector>

std::vector<UI::Vertex> generateSquare(int x, int y, int w, int h, const glm::vec3& color);
std::vector<UI::Vertex> SelectionRectangle(const glm::vec2& pointA, const glm::vec2& pointB, const glm::vec3& color);

#endif
