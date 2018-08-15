#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Mesh {
    struct Vertex {
        glm::vec3 p;
        glm::vec3 n;
        glm::vec3 c;
    };
    typedef std::vector<Vertex> TStaticMesh;
}
namespace UI {
    struct Vertex {
        Vertex(glm::vec2 pos, glm::vec3 col)
        : p(pos)
        , c(col)
        {
        }
        glm::vec2 p;
        glm::vec3 c;
};
}
