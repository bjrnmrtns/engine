#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Camera.h"

void Camera::Move(float forward, float left)
{
    glm::vec3 direction = CalculateDirectionToMoveIn(around, updown);
    glm::vec3 leftdirection(-direction.z, direction.y, direction.x);
    position += direction * -forward;
    position += leftdirection * left;
}
void Camera::MouseMove(float around_diff, float updown_diff)
{
    updown -= updown_diff;
    around += around_diff;
    updown = std::min(pi_2 - 0.1f, std::max(-pi_2 + 0.1f, updown));
}
const glm::mat4 Camera::GetView()
{
    glm::mat4 arcBallRotation = ArcBallRotation(around, updown);
    glm::vec3 directiontopos(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) * arcBallRotation);
    glm::vec3 upvector(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * arcBallRotation);
    return glm::lookAt((glm::normalize(directiontopos) * glm::vec3(zoom, zoom, zoom)) + position, position, glm::normalize(upvector));
}
glm::vec3 Camera::CalculateDirectionToMoveIn(float around, float updown)
{
    glm::vec4 direction = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) * ArcBallRotation(around, updown);
    direction.y = 0.0f; // remove y component, we are only interested in plane movement
    return glm::normalize(glm::vec3(direction));
}
glm::mat4 Camera::ArcBallRotation(float around, float updown)
{
    return glm::rotate(glm::mat4(1.0f), updown, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), around, glm::vec3(0.0f, 1.0f, 0.0f));
}