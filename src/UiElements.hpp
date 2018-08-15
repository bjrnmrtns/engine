#pragma once

#include "VertexDefinitions.hpp"
#include <vector>

std::vector<UI::Vertex> generateSquare(int x, int y, int w, int h,
                                       const glm::vec3 &color);
std::vector<UI::Vertex> SelectionRectangle(const glm::vec2 &pointA,
                                           const glm::vec2 &pointB,
                                           const glm::vec3 &color);
