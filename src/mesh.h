#pragma once

#include <vector>

#include "vertex.h"
#include "triangle.h"

#define CACHED_TRIANGLES 1

namespace ed
{
class Mesh
{
public:
    Mesh(const char* filepath);

#ifdef CACHED_TRIANGLES
    std::vector<Triangle>& getTriangles();
#else
    std::vector<Triangle> getTriangles();
#endif

private:
    std::vector<Triangle> buildTriangles();

private:
    struct Face
    {
        uint32_t positions[3];
        uint32_t normals[3];
        uint32_t uvs[3];
    };

private:
    std::vector<glm::vec3> mPositions;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mUVs;

    std::vector<Face> mFaces;

#ifdef CACHED_TRIANGLES
    std::vector<Triangle> mTriangles;
#endif
};
} // namespace ed
