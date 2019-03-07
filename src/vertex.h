#pragma once

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "vector.h"

namespace ed
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
    };
}

#endif
