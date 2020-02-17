#pragma once

#include "vertex.h"

namespace ed
{
struct Triangle
{
    Triangle() {}
    Triangle(Vertex v1, Vertex v2, Vertex v3)
        : v1(v1)
        , v2(v2)
        , v3(v3)
    {
    }

    Vertex v1;
    Vertex v2;
    Vertex v3;
};
} // namespace ed