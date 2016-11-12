#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "VertexDefinitions.h"

namespace Model
{
    const ::Mesh::TStaticMesh Box();
    const ::Mesh::TStaticMesh Scale(::Mesh::TStaticMesh mesh, const glm::vec3 factor);
}

#endif