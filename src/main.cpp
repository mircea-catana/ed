#include <iostream>

#include "aabb.h"
#include "image.h"
#include "mesh.h"
#include "triangle.h"
#include "vertex.h"

void printVec3(const glm::vec3& v)
{
    std::cout << "V: " << v.x << " " << v.y << " " << v.z << std::endl;
}

int main()
{
    // ed::Mesh mesh = ed::Mesh("/work/projects/softwarerenderer/assets/suzanne.obj");
    // auto& triangles = mesh.getTriangles();

    // std::cout << triangles[0].v1.position.x << " "
    //           << triangles[0].v1.position.y << " "
    //           << triangles[0].v1.position.z << std::endl;

    // ed::Image image = ed::Image("/work/projects/softwarerenderer/assets/dog.jpg");
    // std::cout << image.width() << " " << image.height() << std::endl;
    // image.store("/work/projects/softwarerenderer/dog.png", ed::Image::ImageType::ePng);

    // ed::Image image2 = ed::Image(20, 20);
    // image2.clear(ed::Color(255, 0, 0, 255));
    // image2.store("/work/projects/softwarerenderer/red.png", ed::Image::ImageType::eJpg);

    ed::Vertex v1(glm::vec3(0.0, 2.0, 0.0));
    ed::Vertex v2(glm::vec3(0.0, 0.0, 0.0));
    ed::Vertex v3(glm::vec3(1.0, 0.0, 0.0));

    ed::Triangle t(v1, v2, v3);

    glm::vec3 bary = t.barycentricCoordinates(glm::vec3(1.0, 0.0, 0.0));
    printVec3(bary);

    ed::AABB aabb = t.aabb();
    printVec3(aabb.min());
    printVec3(aabb.max());

    glm::vec3 normal = t.normal();
    printVec3(normal);

    return 0;
}
