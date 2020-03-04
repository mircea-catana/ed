#pragma once

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

namespace ed
{
class AABB
{
public:
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);

    bool isValid() const;
    void extend(const glm::vec3& point);

    glm::vec3 min() const;
    glm::vec3 max() const;

private:
    glm::vec3 mMin;
    glm::vec3 mMax;
};
} // namespace ed
