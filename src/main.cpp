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
    ed::Color grey(80, 80, 80, 255);
    ed::Image output(kWidth, kHeight);
    output.clear(grey);

    ed::Mesh  mesh("/data/projects/rendering/ed/assets/FlareGun.obj");
    ed::Image texture("/data/projects/rendering/ed/assets/FlareGun.png");

    const float aspect = kWidth / kHeight;
    ed::Camera camera(45.0f, aspect, 0.1f, 600.0f);
    camera.lookAt(glm::vec3(0.0f, 0.0f, -10.0f),
                  glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = camera.getView();
    glm::mat4 proj  = camera.getProjection();

    std::vector<float> zBuffer(kWidth * kHeight);
    std::fill(zBuffer.begin(), zBuffer.end(), -std::numeric_limits<float>::max());

    std::vector<ed::Triangle>& triangles = mesh.getTriangles();
    for (ed::Triangle t : triangles) {
        glm::vec4 p1 = glm::vec4(t.v1.position.x, t.v1.position.y, t.v1.position.z, 1.0f);
        glm::vec4 p2 = glm::vec4(t.v2.position.x, t.v2.position.y, t.v2.position.z, 1.0f);
        glm::vec4 p3 = glm::vec4(t.v3.position.x, t.v3.position.y, t.v3.position.z, 1.0f);

        p1 = proj * view * model * p1;
        p2 = proj * view * model * p2;
        p3 = proj * view * model * p3;

        t.v1.position = ndcToScreen(p1);
        t.v2.position = ndcToScreen(p2);
        t.v3.position = ndcToScreen(p3);

        t.computeAABB();

        t.v1.uv *= glm::vec2(texture.width(), texture.height());
        t.v2.uv *= glm::vec2(texture.width(), texture.height());
        t.v3.uv *= glm::vec2(texture.width(), texture.height());

        ed::drawTriangleTextured(output, zBuffer, t, texture);
    }

    output.store("/data/projects/rendering/ed/build/test.png", ed::Image::ImageType::ePng);

    return 0;
}
