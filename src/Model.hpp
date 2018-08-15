#pragma once

#include "VertexDefinitions.hpp"
#include <vector>

namespace Model
{
    const ::Mesh::TStaticMesh Box();
    const ::Mesh::TStaticMesh Grid();
    const ::Mesh::TStaticMesh SimpleTank();

    const ::Mesh::TStaticMesh Scale(::Mesh::TStaticMesh mesh, const glm::vec3 factor);
    const ::Mesh::TStaticMesh Move(::Mesh::TStaticMesh mesh, const glm::vec3 translation);

    const ::Mesh::TStaticMesh ChangeColor(::Mesh::TStaticMesh mesh, glm::vec3 color);
    const ::Mesh::TStaticMesh AdjustColor(::Mesh::TStaticMesh mesh, glm::vec3 color);
}
