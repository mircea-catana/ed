#pragma once

#include <cmath>
#include <vector>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include "aabb.h"
#include "image.h"
#include "triangle.h"

namespace ed
{

bool isZero(float value);

template <typename ColorT>
void drawLine(Image<ColorT>& image, const glm::vec2& p1, const glm::vec2& p2, const ColorT& color)
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

template <typename ColorT>
void drawTriangleWireframe(Image<ColorT>& image, const Triangle& triangle, const ColorT& color)
{
    drawLine(image, triangle.v1.position.xy(), triangle.v2.position.xy(), color);
    drawLine(image, triangle.v2.position.xy(), triangle.v3.position.xy(), color);
    drawLine(image, triangle.v3.position.xy(), triangle.v1.position.xy(), color);
}

template <typename ColorT>
void drawTriangleFill(Image<ColorT>& image, const Triangle& triangle, const ColorT& color)
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

template <typename ColorT>
void drawTriangleTextured(Image<ColorT>& image, Image<ColorR>& zBuffer, const Triangle& triangle, const Image<ColorT>& texture)
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

            // TODO: This is borked
            float depth = triangle.v1.position.z * barycentric.x +
                          triangle.v2.position.z * barycentric.y +
                          triangle.v3.position.z * barycentric.z;

            if (zBuffer.getTexel(i, j) > depth)
                continue;

            zBuffer.setTexel(i, j, depth);

            // Interpolate vertex UVs
            glm::vec2 uv = triangle.v1.uv * barycentric.x +
                           triangle.v2.uv * barycentric.y +
                           triangle.v3.uv * barycentric.z;

            const ColorT& texel = texture.getTexel(uv.x, uv.y);
            image.setTexel(i, j, texel);
        }
    }
}
} // namespace ed
