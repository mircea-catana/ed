#include <iostream>

#include "mesh.h"
#include "image.h"

int main()
{
    ed::Mesh mesh = ed::Mesh("/work/projects/softwarerenderer/assets/suzanne.obj");
    auto& triangles = mesh.getTriangles();

    std::cout << triangles[0].v1.position.x << " "
              << triangles[0].v1.position.y << " "
              << triangles[0].v1.position.z << std::endl;

    ed::Image image = ed::Image("/work/projects/softwarerenderer/assets/dog.jpg");
    std::cout << image.width() << " " << image.height() << std::endl;
    image.store("/work/projects/softwarerenderer/dog.png", ed::Image::ImageType::ePng);

    ed::Image image2 = ed::Image(20, 20);
    image2.clear(ed::Color(255, 0, 0, 255));
    image2.store("/work/projects/softwarerenderer/red.png", ed::Image::ImageType::eJpg);

    return 0;
}
