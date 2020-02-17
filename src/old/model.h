#pragma once

#include "ed.h"
#include "vector.h"

#include <sstream>

namespace ed
{
    class Model
    {
    public:
        Model(const char* filename);

        uint32_t numVertices() const;
        uint32_t numFaces()    const;

        Vector3                vertex(uint32_t i);
        std::vector<uint32_t>& face(uint32_t i);

    private:
        std::vector<Vector3>                mVertices;
        std::vector<std::vector<uint32_t> > mFaces;
    };

    Model::Model(const char* filename)
        : mVertices()
        , mFaces()
    {
        std::ifstream fileStream;
        fileStream.open(filename, std::ifstream::in);
        if (fileStream.fail()) {
            return;
        }

        std::string line;
        while(!fileStream.eof()) {
            std::getline(fileStream, line);
            std::istringstream stringStream(line.c_str());

            char cPlaceholder;
            int  iPlaceholder;

            if (!line.compare(0, 2, "v ")) {
                stringStream >> cPlaceholder;

                Vector3 vertex;
                stringStream >> vertex[0] >> vertex[1] >> vertex[2];

                mVertices.push_back(vertex);

            } else if (!line.compare(0, 2, "f ")) {
                std::vector<uint32_t> face;
                uint32_t index;

                stringStream >> cPlaceholder;
                while (stringStream >> index >> cPlaceholder >> iPlaceholder >> cPlaceholder >> iPlaceholder) {
                    face.push_back(--index);
                }

                mFaces.push_back(face);
            }
        }

        fileStream.close();

        std::cout << "Loaded file - " << filename << " - vertices: " << mVertices.size() << " faces: " << mFaces.size() << std::endl;
    }

    uint32_t Model::numVertices() const
    {
        return mVertices.size();
    }

    uint32_t Model::numFaces() const
    {
        return mFaces.size();
    }

    Vector3 Model::vertex(uint32_t i)
    {
        return mVertices[i];
    }

    std::vector<uint32_t>& Model::face(uint32_t i)
    {
        return mFaces[i];
    }
}
