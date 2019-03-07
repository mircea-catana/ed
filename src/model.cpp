#include "model.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace ed
{
    bool Model::loadObj(const char* filepath)
    {
        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector2> uvs;
        std::vector<Vector3> vertexIndices;

        std::ifstream fileStream;
        fileStream.open(filepath, std::ifstream::in);
        if (fileStream.fail())
            return false;

        std::string line;
        while (!fileStream.eof()) {
            std::getline(fileStream, line);
            std::istringstream stringStream(line.c_str());

            char discard[2];

            if (!line.compare(0, 2, "v ")) {
                Vector3 position;

                stringStream >> discard[0];
                stringStream >> position[0] >> position[1] >> position[2];

                positions.push_back(position);

            } else if (!line.compare(0, 3, "vn ")) {
                Vector3 normal;

                stringStream >> discard;
                stringStream >> normal[0] >> normal[1] >> normal[2];

                normals.push_back(normal);

            } else if (!line.compare(0, 3, "vt ")) {
                Vector2 uv;

                stringStream >> discard;
                stringStream >> uv[0] >> uv[1];

                uvs.push_back(uv);

            } else if (!line.compare(0, 2, "f ")) {
                Vector3 vertexIndex;

                stringStream >> discard[0];

                while (stringStream >> vertexIndex[0] >> discard[0] >> vertexIndex[2] >> discard[0] >> vertexIndex[1])
                    vertexIndices.push_back(vertexIndex);
            }
        }

        fileStream.close();

        for (Vector3 vi : vertexIndices) {
            Vertex vertex;
            vertex.position = positions[vi[0] - 1];
            vertex.normal   = normals[vi[1] - 1];
            vertex.uv       = uvs[vi[2] - 1];

            mVertices.push_back(vertex);
        }

        return true;
    }

    std::vector<Vertex>& Model::vertices()
    {
        return mVertices;
    }
}
