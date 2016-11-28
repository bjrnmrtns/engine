#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <glm/glm.hpp>

namespace Configuration
{
struct EntityDefinition
{
    glm::vec3 startpos;
};

static EntityDefinition EntityDefinitions[] {
    { glm::vec3(-6, 0, -6) },
    { glm::vec3(9, 0, 9) },
    { glm::vec3(-3, 0, -3) },
    { glm::vec3(6, 0, 6) },
    { glm::vec3(3, 0, 3) }
};
}
#endif
