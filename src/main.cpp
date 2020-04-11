#include <iostream>
#include <vector>
#include <memory>

#include "aabb.h"
#include "camera.h"
#include "framebuffer.h"
#include "image.h"
#include "mesh.h"
#include "rasterizer.h"
#include "triangle.h"
#include "vertex.h"

#include <glm/gtc/matrix_transform.hpp>

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
    //---------------------- FRAMEBUFFER ------------------------
    ed::ColorAttachmentT colorA(new ed::Image<ed::ColorRGBA>(kWidth, kHeight));
    colorA->clear(ed::ColorRGBA(80, 80, 80, 255));

    ed::DSAttachmentT depthA(new ed::Image<ed::ColorR>(kWidth, kHeight));
    depthA->clear(ed::ColorR(std::numeric_limits<float>::max()));

    ed::Framebuffer framebuffer(colorA, depthA);

    //---------------------- MODEL ------------------------
    ed::Mesh  mesh("/data/projects/rendering/ed/assets/FlareGun.obj");
    ed::Image<ed::ColorRGBA> texture("/data/projects/rendering/ed/assets/FlareGun.png");

    //---------------------- CAMERA ------------------------
    const float aspect = kWidth / kHeight;
    ed::Camera camera(45.0f, aspect, 0.1f, 150.0f);
    camera.lookAt(glm::vec3(0.75f, 0.4f, -1.0f),
                  glm::vec3(-0.5f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 2.5f));
    glm::mat4 view  = camera.getView();
    glm::mat4 proj  = camera.getProjection();
    glm::mat4 MVP   = proj * view * model;

    //---------------------- SHADER ------------------------
    glm::vec4 light = MVP * glm::vec4(2.0f, 1.0f, 4.0f, 1.0);
    ed::SimpleShader shader;
    shader.MVP             = &MVP;
    shader.texture         = &texture;
    shader.uLightDirection = glm::normalize(light.xyz());

    //---------------------- RENDER ------------------------
    std::vector<ed::Triangle>& triangles = mesh.getTriangles();
    for (ed::Triangle& t : triangles) {
        ed::drawTriangle(framebuffer, t, &shader);
    }

    //---------------------- WRITE TO DISK ------------------------
    colorA->store("/data/projects/rendering/ed/build/test.png", ed::ImageType::ePng);

    return 0;
}
