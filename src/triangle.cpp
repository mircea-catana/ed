#include "triangle.h"

#include <algorithm>

namespace ed
{

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    : v1(v1)
    , v2(v2)
    , v3(v3)
{
    computeAABB();
}

glm::vec3 Triangle::normal() const
{
    glm::vec3 edge1 = v2.position - v1.position;
    glm::vec3 edge2 = v3.position - v1.position;
    return glm::normalize(glm::cross(edge1, edge2));
}

glm::vec3 Triangle::normal(const glm::vec3& point) const
{
    glm::vec3 bc = barycentric(point);
    return v1.normal * bc.x +
           v2.normal * bc.y +
           v3.normal * bc.z;
}

glm::vec2 Triangle::uv(const glm::vec3& point) const
{
    glm::vec3 bc = barycentric(point);
    return v1.uv * bc.x +
           v2.uv * bc.y +
           v3.uv * bc.z;
}

glm::vec3 Triangle::barycentric(const glm::vec3& point) const
{
    const glm::vec3& p1 = v1.position;
    const glm::vec3& p2 = v2.position;
    const glm::vec3& p3 = v3.position;

    float det = (p1.x - p3.x) * (p2.y - p3.y) + (p3.x - p2.x) * (p1.y - p3.y);

    float u   = (point.x - p3.x) * (p2.y - p3.y) + (p3.x - p2.x) * (point.y - p3.y);
    float v   = (point.x - p3.x) * (p3.y - p1.y) + (p1.x - p3.x) * (point.y - p3.y);

    u /= det;
    v /= det;

    float w = 1.0f - u - v;

    // Dealing with fp precision
    if (std::abs(w) < 1e-6f)
        w = 0.0;

    return glm::vec3(u, v, w);
}

glm::vec3 Triangle::barycentric(const glm::vec2& p1, const glm::vec2& p2,
                                const glm::vec2& p3, const glm::vec2& point) const
{
    float det = (p1.x - p3.x) * (p2.y - p3.y) + (p3.x - p2.x) * (p1.y - p3.y);

    float u   = (point.x - p3.x) * (p2.y - p3.y) + (p3.x - p2.x) * (point.y - p3.y);
    float v   = (point.x - p3.x) * (p3.y - p1.y) + (p1.x - p3.x) * (point.y - p3.y);

    u /= det;
    v /= det;

    float w = 1.0f - u - v;

    // Dealing with fp precision
    if (std::abs(w) < 1e-6f)
        w = 0.0;

    return glm::vec3(u, v, w);
}

AABB Triangle::aabb() const
{
    return mBbox;
}

void Triangle::computeAABB()
{
    glm::vec3 min = v1.position;
    glm::vec3 max = v1.position;

    float minX = std::min(v2.position.x, v3.position.x);
    float minY = std::min(v2.position.y, v3.position.y);
    float minZ = std::min(v2.position.z, v3.position.z);

    min.x = std::min(min.x, minX);
    min.y = std::min(min.y, minY);
    min.z = std::min(min.z, minZ);

    float maxX = std::max(v2.position.x, v3.position.x);
    float maxY = std::max(v2.position.y, v3.position.y);
    float maxZ = std::max(v2.position.z, v3.position.z);

    max.x = std::max(max.x, maxX);
    max.y = std::max(max.y, maxY);
    max.z = std::max(max.z, maxZ);

    mBbox = AABB(min, max);
}


Vertex Triangle::operator [] (size_t index) const
{
    if (index == 0) {
        return v1;
    } else if (index == 1) {
        return v2;
    }

    return v3;
}

Vertex& Triangle::operator [] (size_t index)
{
    if (index == 0) {
        return v1;
    } else if (index == 1) {
        return v2;
    }

    return v3;
}


} // namespace ed
