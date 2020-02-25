#pragma once

#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>

#include "image.h"
#include "triangle.h"

namespace ed
{
    void drawLine(Image& image, const glm::vec2& p1, const glm::vec2& p2, const Color& color);

    // Triangle functions assume triangles have been flattened
    // Pass triangles only after they have been projected
    void drawTriangleWireframe(Image& image, const Triangle& triangle, const Color& color);
    void drawTriangleFill(Image& image, const Triangle& triangle, const Color& color);
    void drawTriangleTextured(Image& image, std::vector<float>& zBuffer, const Triangle& triangle, const Image& texture);

} // namespace ed
