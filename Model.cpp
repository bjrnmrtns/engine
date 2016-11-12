#include "Model.h"

namespace Model {
    const glm::vec3 A(-1.0f, -1.0f,  1.0f);
    const glm::vec3 B( 1.0f, -1.0f,  1.0f);
    const glm::vec3 C(-1.0f,  1.0f,  1.0f);
    const glm::vec3 D( 1.0f,  1.0f,  1.0f);
    const glm::vec3 E(-1.0f, -1.0f, -1.0f);
    const glm::vec3 F( 1.0f, -1.0f, -1.0f);
    const glm::vec3 G(-1.0f,  1.0f, -1.0f);
    const glm::vec3 H( 1.0f,  1.0f, -1.0f);
    const glm::vec3 XPlus( 1.0f,  0.0f,  0.0f);
    const glm::vec3 XMin (-1.0f,  0.0f,  0.0f);
    const glm::vec3 YPlus( 0.0f,  1.0f,  0.0f);
    const glm::vec3 YMin ( 0.0f, -1.0f,  0.0f);
    const glm::vec3 ZPlus( 0.0f,  0.0f,  1.0f);
    const glm::vec3 ZMin ( 0.0f,  0.0f, -1.0f);
    const ::Mesh::TStaticMesh XYPlusPlane { {A, ZPlus}, {C, ZPlus}, {B, ZPlus},
                                            {B, ZPlus}, {C, ZPlus}, {D, ZPlus} };
    const ::Mesh::TStaticMesh XYMinPlane  { {G, ZMin}, {E, ZMin}, {F, ZMin},
                                            {G, ZMin}, {F, ZMin}, {H, ZMin} };
    const ::Mesh::TStaticMesh XZPlusPlane { {C, YPlus}, {G, YPlus}, {D, YPlus},
                                            {D, YPlus}, {G, YPlus}, {H, YPlus} };
    const ::Mesh::TStaticMesh XZMinPlane  { {E, YMin}, {A, YMin}, {B, YMin},
                                            {E, YMin}, {B, YMin}, {F, YMin} };
    const ::Mesh::TStaticMesh YZPlusPlane { {B, XPlus}, {D, XPlus}, {F, XPlus},
                                            {F, XPlus}, {D, XPlus}, {H, XPlus} };
    const ::Mesh::TStaticMesh YZMinPlane  { {C, XMin}, {A, XMin}, {E, XMin},
                                            {C, XMin}, {E, XMin}, {G, XMin} };
}

const ::Mesh::TStaticMesh Model::Box()
{
    ::Mesh::TStaticMesh box(XYPlusPlane);
    box.insert(box.end(), XZPlusPlane.begin(), XZPlusPlane.end());
    box.insert(box.end(), YZPlusPlane.begin(), YZPlusPlane.end());
    box.insert(box.end(), XYMinPlane.begin(),  XYMinPlane.end());
    box.insert(box.end(), XZMinPlane.begin(),  XZMinPlane.end());
    box.insert(box.end(), YZMinPlane.begin(),  YZMinPlane.end());
    return box;
}