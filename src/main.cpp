#include <iostream>
#include <vector>

#include "aabb.h"
#include "camera.h"
#include "image.h"
#include "mesh.h"
#include "rasterizer.h"
#include "triangle.h"
#include "vertex.h"

static const float kWidth  = 1920.0f;
static const float kHeight = 1080.0f;

glm::vec3 ndcToScreen(const glm::vec4& v)
{
    return glm::vec3((v.x + 1) * kWidth * 0.5,
                     (v.y + 1) * kHeight * 0.5,
                     v.z);
}

int main()
{
    ed::ColorRGBA grey(80, 80, 80, 255);
    ed::Image<ed::ColorRGBA> output(kWidth, kHeight);
    output.clear(grey);

    ed::Mesh  mesh("/work/projects/softwarerenderer/assets/Ivysaur.obj");
    ed::Image<ed::ColorRGBA> texture("/work/projects/softwarerenderer/assets/Ivysaur_Diffuse.jpg");

    const float aspect = kWidth / kHeight;
    ed::Camera camera(45.0f, aspect, 0.1f, 600.0f);
    camera.lookAt(glm::vec3(0.0f, -1.0f, 2.5f),
                  glm::vec3(0.0f, 0.5f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = camera.getView();
    glm::mat4 proj  = camera.getProjection();

    ed::Image<ed::ColorR> zBuffer(kWidth, kHeight);
    zBuffer.clear(ed::ColorR(-std::numeric_limits<float>::max()));

    std::vector<ed::Triangle>& triangles = mesh.getTriangles();
    for (ed::Triangle t : triangles) {
        glm::vec4 p1 = glm::vec4(t.v1.position.x, t.v1.position.y, t.v1.position.z, 1.0f);
        glm::vec4 p2 = glm::vec4(t.v2.position.x, t.v2.position.y, t.v2.position.z, 1.0f);
        glm::vec4 p3 = glm::vec4(t.v3.position.x, t.v3.position.y, t.v3.position.z, 1.0f);

        p1 = proj * view * model * p1;
        p2 = proj * view * model * p2;
        p3 = proj * view * model * p3;

        t.v1.position = ndcToScreen(p1/p1.w);
        t.v2.position = ndcToScreen(p2/p2.w);
        t.v3.position = ndcToScreen(p3/p3.w);

        t.computeAABB();

        t.v1.uv *= glm::vec2(texture.width(), texture.height());
        t.v2.uv *= glm::vec2(texture.width(), texture.height());
        t.v3.uv *= glm::vec2(texture.width(), texture.height());

        ed::drawTriangleTextured<ed::ColorRGBA>(output, zBuffer, t, texture);
    }

    output.store("/work/projects/softwarerenderer/build/test.png", ed::ImageType::ePng);

    return 0;
}
