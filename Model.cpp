#include "Model.h"

namespace Model {
    static glm::vec3 A(-1.0f, -1.0f,  1.0f);
    static glm::vec3 B( 1.0f, -1.0f,  1.0f);
    static glm::vec3 C(-1.0f,  1.0f,  1.0f);
    static glm::vec3 D( 1.0f,  1.0f,  1.0f);
    static glm::vec3 E(-1.0f, -1.0f, -1.0f);
    static glm::vec3 F( 1.0f, -1.0f, -1.0f);
    static glm::vec3 G(-1.0f,  1.0f, -1.0f);
    static glm::vec3 H( 1.0f,  1.0f, -1.0f);
    static glm::vec3 XUp  ( 1.0f,  0.0f,  0.0f);
    static glm::vec3 XDown(-1.0f,  0.0f,  0.0f);
    static glm::vec3 YUp  ( 0.0f,  1.0f,  0.0f);
    static glm::vec3 YDown( 0.0f, -1.0f,  0.0f);
    static glm::vec3 ZUp  ( 0.0f,  0.0f,  1.0f);
    static glm::vec3 ZDown( 0.0f,  0.0f, -1.0f);
}

const std::vector<::Mesh::Vertex> Model::XZPlusPlane()
{
    return {
            {A, YUp}, {E, YUp}, {B, YUp},
            {B, YUp}, {E, YUp}, {F, YUp}
    };
}