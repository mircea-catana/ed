#pragma once

#include "ed.h"
#include "vector.h"
#include "framebuffer.h"

namespace ed
{
    /*******************************************************************************
    *   Bresenham Line Drawing algorithm                                           *
    *******************************************************************************/
    void drawLine(Image& image, const Vector2 p1, const Vector2 p2, const Color color)
    {
        const float dx = p2.x() - p1.x();
        const float dy = p2.y() - p1.y();

        if (dx == 0.0f && dy == 0.0f) {
            return;
        }

        if (fabsf(dx) > fabsf(dy)) {
            float minX, maxX;

            if (p1.x() < p2.x()) {
                minX = p1.x();
                maxX = p2.x();
            } else {
                minX = p2.x();
                maxX = p1.x();
            }

            const float slope = dy / dx;
            for (float x = minX; x <= maxX; ++x) {
                float y = p1.y() + (x - p1.x()) * slope;
                image.set(x, y, color);
            }

        } else {
            float minY, maxY;

            if (p1.y() < p2.y()) {
                minY = p1.y();
                maxY = p2.y();
            } else {
                minY = p2.y();
                maxY = p1.y();
            }

            const float slope = dx / dy;
            for (float y = minY; y <= maxY; ++y) {
                float x = p1.x() + (y - p1.y()) * slope;
                image.set(x, y, color);
            }
        }
    }
}
