#include "aabb.h"

#include <algorithm>
#include <limits>

namespace ed
{

AABB::AABB()
    : mMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
    , mMax(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min())
{
}

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
    : mMin(min)
    , mMax(max)
{
}

bool AABB::isValid() const
{
    return mMin.x <= mMax.x && mMin.y <= mMax.y && mMin.z <= mMax.z;
}

void AABB::extend(const glm::vec3& point)
{
    if (!isValid()) {
        mMin = point;
        mMax = point;
        return;
    }

    mMin.x = std::min(mMin.x, point.x);
    mMin.y = std::min(mMin.y, point.y);
    mMin.z = std::min(mMin.z, point.z);

    mMax.x = std::max(mMax.x, point.x);
    mMax.y = std::max(mMax.y, point.y);
    mMax.z = std::max(mMax.z, point.z);
}

AABB AABB::intersect(const AABB& other) const
{
    glm::vec3 min = glm::max(mMin, other.min());
    glm::vec3 max = glm::min(mMax, other.max());

    return AABB(min, max);
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
