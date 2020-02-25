#pragma once

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

namespace ed
{
class Camera
{
public:
    Camera(float fov, float aspect, float near, float far);

    void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

    glm::mat4 getView() const;
    glm::mat4 getProjection() const;

private:

    float mFov;
    float mAspect;
    float mNear;
    float mFar;

    glm::mat4 mView;
    glm::mat4 mProjection;
};
} // namespace ed
