#ifndef FONT_H
#define FONT_H

#include <glm/glm.hpp>
#include <vector>
#include "VertexDefinitions.h"

bool GenerateText(const char text[], std::vector<Text::Vertex>& textbuf);

#endif
