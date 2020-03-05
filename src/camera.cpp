#include "camera.h"

#include <glm/gtx/transform.hpp>

namespace ed
{

Camera::Camera(float fov, float aspect, float near, float far)
    : mFov(fov)
    , mAspect(aspect)
    , mNear(near)
    , mFar(far)
{
    mView       = glm::mat4(1.0);
    mProjection = glm::perspective(mFov, mAspect, mNear, mFar);
}

void Camera::lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
    mView = glm::lookAt(position, target, up);
}

glm::mat4 Camera::getView() const
{
    return mView;
}

glm::mat4 Camera::getProjection() const
{
    return mProjection;
}
} // namespace ed
