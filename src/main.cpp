#include <iostream>

#include "mesh.h"

int main()
{
    ed::Mesh mesh = ed::Mesh("/work/projects/softwarerenderer/assets/suzanne.obj");
    auto& triangles = mesh.getTriangles();

    std::cout << triangles[0].v1.position.x << " "
              << triangles[0].v1.position.y << " "
              << triangles[0].v1.position.z << std::endl;

    return 0;
}
