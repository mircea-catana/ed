#include "mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace ed
{

Mesh::Mesh(const char* filepath)
{
    std::ifstream fileStream;
    fileStream.open(filepath, std::ifstream::in);
    if (fileStream.fail()) {
        std::cerr << "Cannot open file: " << filepath << std::endl;
        return;
    }

    mPositions = std::vector<glm::vec3>();
    mNormals   = std::vector<glm::vec3>();
    mUVs       = std::vector<glm::vec2>();
    mFaces     = std::vector<Face>();

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        std::istringstream stringStream(line.c_str());

        if (!line.compare(0, 2, "v ")) {
            char placeholder;
            glm::vec3 position;

            stringStream >> placeholder;
            stringStream >> position[0] >> position[1] >> position[2];

            mPositions.push_back(position);

        } else if (!line.compare(0, 3, "vn ")) {
            char placeholder;
            glm::vec3 normal;

            stringStream >> placeholder >> placeholder;
            stringStream >> normal[0] >> normal[1] >> normal[2];

            mNormals.push_back(normal);

        } else if (!line.compare(0, 3, "vt ")) {
            char placeholder;
            glm::vec2 uv;

            stringStream >> placeholder >> placeholder;
            stringStream >> uv[0] >> uv[1];

            mUVs.push_back(uv);

        } else if (!line.compare(0, 2, "f ")) {
            char placeholder;
            Face face;

            stringStream >> placeholder;
            for (uint32_t i = 0; i < 3; ++i) {
                stringStream >> face.positions[i] >> placeholder >> face.uvs[i] >> placeholder >> face.normals[i];

                // OBJ files are indexed from 1
                --face.positions[i];
                --face.normals[i];
                --face.uvs[i];
            }

            mFaces.push_back(face);
        }
    }

    fileStream.close();

#ifdef CACHED_TRIANGLES
    mTriangles = buildTriangles();
#endif
}

std::vector<Triangle> Mesh::buildTriangles()
{
    std::vector<Triangle> triangles;

    for (Face& face : mFaces) {
        Vertex v1;
        v1.position = mPositions[face.positions[0]];
        v1.normal   = mNormals[face.normals[0]];
        v1.uv       = mUVs[face.uvs[0]];

        Vertex v2;
        v2.position = mPositions[face.positions[1]];
        v2.normal   = mNormals[face.normals[1]];
        v2.uv       = mUVs[face.uvs[1]];

        Vertex v3;
        v3.position = mPositions[face.positions[2]];
        v3.normal   = mNormals[face.normals[2]];
        v3.uv       = mUVs[face.uvs[2]];

        triangles.push_back(Triangle(v1, v2, v3));
    }

    return triangles;
}

#ifdef CACHED_TRIANGLES
std::vector<Triangle>& Mesh::getTriangles()
{
    return mTriangles;
}
#else
std::vector<Triangle> Mesh::getTriangles()
{
    return buildTriangles();
}
#endif

} // namespace ed
