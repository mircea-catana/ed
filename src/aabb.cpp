#include "aabb.h"

namespace ed
{

AABB::AABB()
    : mMin(0.0f, 0.0f, 0.0f)
    , mMax(0.0f, 0.0f, 0.0f)
{
}

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
    : mMin(min)
    , mMax(max)
{
}

glm::vec3 AABB::min() const
{
    return mMin;
}

glm::vec3 AABB::max() const
{
    return mMax;
}

} // namespace ed
