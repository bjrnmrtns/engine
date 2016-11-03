#ifndef VERTEX_DEFINITIONS_H
#define VERTEX_DEFINITIONS_H

#include <glm/glm.hpp>

namespace Mesh {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 col;
    };
}
namespace UI {
    struct Vertex {
        Vertex(glm::vec2 pos, glm::vec3 col)
        : pos(pos)
        , col(col)
        {
        }
        glm::vec2 pos;
        glm::vec3 col;
};
}

namespace Text {
    struct Vertex {
        Vertex(glm::vec2 pos, glm::vec3 col, glm::vec2 texcoord)
         : pos(pos)
         , col(col)
         , texcoord(texcoord)
        {
        }
        glm::vec2 pos;
        glm::vec3 col;
        glm::vec2 texcoord;
    };
}
#endif