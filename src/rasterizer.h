#pragma once

#include <cmath>
#include <vector>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include "aabb.h"
#include "framebuffer.h"
#include "image.h"
#include "shader.h"
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

void drawTriangle(Framebuffer& framebuffer, const Triangle& triangle, Shader* shader);

} // namespace ed
