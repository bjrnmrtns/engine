#include "Model.h"

namespace Model {
    namespace Color {
        const glm::vec3          Red(1.0f, 0.0f, 0.0f);
        const glm::vec3      DarkRed(0.5f, 0.0f, 0.0f);
        const glm::vec3        Green(0.0f, 1.0f, 0.0f);
        const glm::vec3    DarkGreen(0.0f, 0.5f, 0.0f);
        const glm::vec3         Blue(0.0f, 0.0f, 1.0f);
        const glm::vec3     DarkBlue(0.0f, 0.0f, 0.5f);
        const glm::vec3         Grey(0.3f, 0.3f, 0.3f);
        const glm::vec3  AlmostWhite(0.05f, 0.05f, 0.05f);
    }
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
    const ::Mesh::TStaticMesh XYPlusPlane { {A, ZPlus, Color::Blue}, {C, ZPlus, Color::Blue}, {B, ZPlus, Color::Blue},
                                            {B, ZPlus, Color::Blue}, {C, ZPlus, Color::Blue}, {D, ZPlus, Color::Blue} };
    const ::Mesh::TStaticMesh XYMinPlane  { {G, ZMin, Color::DarkBlue}, {E, ZMin, Color::DarkBlue}, {F, ZMin, Color::DarkBlue},
                                            {G, ZMin, Color::DarkBlue}, {F, ZMin, Color::DarkBlue}, {H, ZMin, Color::DarkBlue} };
    const ::Mesh::TStaticMesh XZPlusPlane { {C, YPlus, Color::Green}, {G, YPlus, Color::Green}, {D, YPlus, Color::Green},
                                            {D, YPlus, Color::Green}, {G, YPlus, Color::Green}, {H, YPlus, Color::Green} };
    const ::Mesh::TStaticMesh XZMinPlane  { {E, YMin, Color::DarkGreen}, {A, YMin, Color::DarkGreen}, {B, YMin, Color::DarkGreen},
                                            {E, YMin, Color::DarkGreen}, {B, YMin, Color::DarkGreen}, {F, YMin, Color::DarkGreen} };
    const ::Mesh::TStaticMesh YZPlusPlane { {B, XPlus, Color::Red}, {D, XPlus, Color::Red}, {F, XPlus, Color::Red},
                                            {F, XPlus, Color::Red}, {D, XPlus, Color::Red}, {H, XPlus, Color::Red} };
    const ::Mesh::TStaticMesh YZMinPlane  { {C, XMin, Color::DarkRed}, {A, XMin, Color::DarkRed}, {E, XMin, Color::DarkRed},
                                            {C, XMin, Color::DarkRed}, {E, XMin, Color::DarkRed}, {G, XMin, Color::DarkRed} };
}

const ::Mesh::TStaticMesh Model::Grid()
{
    ::Mesh::TStaticMesh grid;
    ::Mesh::TStaticMesh oddCell  = ChangeColor(XZPlusPlane, Color::AlmostWhite);
    ::Mesh::TStaticMesh evenCell = ChangeColor(XZPlusPlane, Color::Grey);
    float xColorAdjustment = 0.007f;
    float zColorAdjustment = 0.007f;
    for(int x = 0; x < 100; x++)
    {
        for(int z = 0; z < 100; z++)
        {
            if((x + z) % 2 == 1)
            {
                ::Mesh::TStaticMesh element = Move(AdjustColor(oddCell, glm::vec3(xColorAdjustment * x, 0.0f, zColorAdjustment * z)), glm::vec3(-50 + x*2, -1.0f, -50 + z*2));
                grid.insert(grid.end(), element.begin(), element.end());
            }
            else
            {
                ::Mesh::TStaticMesh element = Move(AdjustColor(evenCell, glm::vec3(xColorAdjustment * x, 0.0f, zColorAdjustment * z)), glm::vec3(-50 + x*2, -1.0f, -50 + z*2));
                grid.insert(grid.end(), element.begin(), element.end());
            }
        }
    }
    return grid;
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

const ::Mesh::TStaticMesh Model::Scale(::Mesh::TStaticMesh mesh, const glm::vec3 factor)
{
    for(auto& item: mesh)
    {
        item.p *= factor;
    }
    return mesh;
}

const ::Mesh::TStaticMesh Model::Move(::Mesh::TStaticMesh mesh, const glm::vec3 translation)
{
    for(auto& item: mesh)
    {
        item.p += translation;
    }
    return mesh;
}

const ::Mesh::TStaticMesh Model::ChangeColor(::Mesh::TStaticMesh mesh, glm::vec3 color)
{
    for(auto& item: mesh)
    {
        item.c = color;
    }
    return mesh;
}

const ::Mesh::TStaticMesh Model::AdjustColor(::Mesh::TStaticMesh mesh, glm::vec3 color)
{
    for(auto& item: mesh)
    {
        item.c += color;
    }
    return mesh;
}