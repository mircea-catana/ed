#include "rasterizer.h"

#include <cmath>

#include "aabb.h"

namespace ed
{

bool isZero(float value)
{
    return (std::abs(value) < 1e-6f) ? true : false;
}

void drawLine(Image& image, const glm::vec2& p1, const glm::vec2& p2, const Color& color)
{
    const float dx = p2.x - p1.x;
    const float dy = p2.y - p1.y;

    if (isZero(dx) && isZero(dy))
        return;

    if (std::abs(dx) > std::abs(dy)) {
        float min, max;

        if (p1.x < p2.x) {
            min = p1.x;
            max = p2.x;
        } else {
            min = p2.x;
            max = p1.x;
        }

        const float slope = dy / dx;
        for (float x = min; x <= max; ++x) {
            float y = p1.y + (x - p1.x) * slope;
            image.setTexel(x, y, color);
        }

    } else {
        float min, max;

        if (p1.y < p2.y) {
            min = p1.y;
            max = p2.y;
        } else {
            min = p2.y;
            max = p1.y;
        }

        const float slope = dx / dy;
        for (float y = min; y <= max; ++y) {
            float x = p1.x + (y - p1.y) * slope;
            image.setTexel(x, y, color);
        }
    }
}

void drawTriangleWireframe(Image& image, const Triangle& triangle, const Color& color)
{
    drawLine(image, triangle.v1.position.xy(), triangle.v2.position.xy(), color);
    drawLine(image, triangle.v2.position.xy(), triangle.v3.position.xy(), color);
    drawLine(image, triangle.v3.position.xy(), triangle.v1.position.xy(), color);
}

void drawTriangleFill(Image& image, const Triangle& triangle, const Color& color)
{
    AABB bbox = triangle.aabb();
    for (size_t j = bbox.min().y; j <= bbox.max().y; ++j) {
        for (size_t i = bbox.min().x; i <= bbox.max().x; ++i) {
            glm::vec3 point = glm::vec3(i, j, 0.0);
            glm::vec3 barycentric = triangle.barycentricCoordinates(point);
            if (barycentric.x >= 0.0 && barycentric.y >= 0.0 && barycentric.z >= 0.0) {
                image.setTexel(i, j, color);
            }
        }
    }
}

void drawTriangleTextured(Image& image, std::vector<float>& zBuffer, const Triangle& triangle, const Image& texture)
{
    AABB bbox = triangle.aabb();

    for (size_t j = bbox.min().y; j <= bbox.max().y; ++j) {
        for (size_t i = bbox.min().x; i <= bbox.max().x; ++i) {

            if (i < 0 || j < 0 || i > image.width()-1 || j > image.height() - 1)
                continue;

            glm::vec3 point = glm::vec3(i, j, 0.0);
            glm::vec3 barycentric = triangle.barycentricCoordinates(point);

            if (barycentric.x < 0.0 || barycentric.y < 0.0 || barycentric.z < 0.0)
                continue;

            float depth = triangle.v1.position.z * barycentric.x +
                          triangle.v2.position.z * barycentric.y +
                          triangle.v3.position.z * barycentric.z;

            if (zBuffer[j * (image.width() - 1) + i] > depth)
                continue;

            zBuffer[j * (image.width() - 1) + i] = depth;

            // Interpolate vertex UVs
            glm::vec2 uv = triangle.v1.uv * barycentric.x +
                           triangle.v2.uv * barycentric.y +
                           triangle.v3.uv * barycentric.z;

            const Color& texel = texture.getTexel(uv.x, uv.y);
            image.setTexel(i, j, texel);
        }
    }
}

} // namespace ed
