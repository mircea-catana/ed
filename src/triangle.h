#pragma once

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include "vertex.h"
#include "aabb.h"

namespace ed
{
class Triangle
{
public:
    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

    glm::vec3 barycentric(const glm::vec3& point) const;
    glm::vec3 barycentric(const glm::vec2& p1, const glm::vec2& p2,
                          const glm::vec2& p3, const glm::vec2& point) const;

    // Flat normal
    glm::vec3 normal() const;
    // Interpolated normal from vertex data
    glm::vec3 normal(const glm::vec3& point) const;

    glm::vec2 uv(const glm::vec3& point) const;

    AABB aabb() const;
    void computeAABB();

    Vertex operator [] (size_t index) const;
    Vertex& operator [] (size_t index);

    Vertex v1;
    Vertex v2;
    Vertex v3;

private:
    AABB mBbox;
};
} // namespace ed
