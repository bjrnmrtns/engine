#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class Camera
{
public:
    static constexpr float pi = glm::pi<float>();
    static constexpr float pi_2 = pi / 2.0f;

    glm::vec3 position = glm::vec3(0.0f, 5.0f, 0.0f);
    float updown = 0.0f;
    float around = 0.0f;
    float zoom = 20.0f;

    void Move(float forward, float left);
    void MouseMove(float around_diff, float updown_diff);
    const glm::mat4 GetView();
private:
    static glm::vec3 CalculateDirectionToMoveIn(float around, float updown);
    // The arcball rotation consists of two parts. The last one does the rotation on the y-axis so around the center on xz plane,
    // the second one is about the updown around the x-axis.
    static glm::mat4 ArcBallRotation(float around, float updown);
};

#endif
