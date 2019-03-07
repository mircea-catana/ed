#pragma once

#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>

#include "vertex.h"

namespace ed
{
    class Model
    {
    public:
        Model()                   = default;
        Model(const Model& other) = delete;
        Model(Model&& other)      = delete;
        ~Model()                  = default;

        Model& operator= (const Model& other) = delete;
        Model& operator= (Model&& other)      = delete;

        bool loadObj(const char* filepath);
        std::vector<Vertex>& vertices();

    private:
        std::vector<Vertex> mVertices;
    };
}

#endif
