#include "rasterizer.h"
#include <iostream>
namespace ed
{

bool isZero(float value)
{
    return (std::abs(value) < 1e-6f) ? true : false;
}

void drawTriangle(Framebuffer& framebuffer, const Triangle& triangle, Shader* shader)
{
    Image<ColorRGBA>& cBuffer = *framebuffer.getColorAttachment(0);
    Image<ColorR>&    zBuffer = *framebuffer.getDepthAttachment();

    //---------------------- VERTEX ------------------------
    shader->vertex(triangle);

    float halfWidth  = cBuffer.width() * 0.5f;
    float halfHeight = cBuffer.height() * 0.5f;

    std::array<glm::vec4, 3> coordinates;
    std::array<glm::vec2, 3> screenCoordinates;
    for (size_t i = 0; i < 3; ++i) {
        glm::vec4& clipCoordinates = shader->clipCoordinates[i];

        coordinates[i].x = halfWidth * (clipCoordinates.w + clipCoordinates.x);
        coordinates[i].y = halfHeight * (clipCoordinates.w + clipCoordinates.y);
        coordinates[i].z = clipCoordinates.w;
        coordinates[i].w = clipCoordinates.w;

        screenCoordinates[i].x = coordinates[i].x / coordinates[i].w;
        screenCoordinates[i].y = coordinates[i].y / coordinates[i].w;
    }

    AABB bbox;
    bbox.extend(glm::vec3(screenCoordinates[0].x, screenCoordinates[0].y, 0.0f));
    bbox.extend(glm::vec3(screenCoordinates[1].x, screenCoordinates[1].y, 0.0f));
    bbox.extend(glm::vec3(screenCoordinates[2].x, screenCoordinates[2].y, 0.0f));

    //---------------------- FRAGMENT ------------------------
    for (size_t j = bbox.min().y; j <= bbox.max().y; ++j) {
        for (size_t i = bbox.min().x; i <= bbox.max().x; ++i) {

            if (i < 0 || j < 0 || i > cBuffer.width()-1 || j > cBuffer.height() - 1)
                continue;

            glm::vec2 point = glm::vec2(i, j);
            glm::vec3 barycentric_screen = triangle.barycentric(screenCoordinates[0], screenCoordinates[1], screenCoordinates[2], point);
            glm::vec3 barycentric_clip   = glm::vec3(barycentric_screen.x / coordinates[0].w,
                                                     barycentric_screen.y / coordinates[1].w,
                                                     barycentric_screen.z / coordinates[2].w);
            barycentric_clip = barycentric_clip / (barycentric_clip.x + barycentric_clip.y + barycentric_clip.z);

            if (barycentric_screen.x < 0.0f || barycentric_screen.y < 0.0f || barycentric_screen.z < 0.0f)
                continue;

            float depth = shader->clipCoordinates[0].z * barycentric_clip.x +
                          shader->clipCoordinates[1].z * barycentric_clip.y +
                          shader->clipCoordinates[2].z * barycentric_clip.z;

            if (zBuffer.getTexel(i, j) > depth)
                continue;

            zBuffer.setTexel(i, j, depth);

            const ColorRGBA color = shader->fragment(barycentric_clip);
            cBuffer.setTexel(i, j, color);
        }
    }
}

} // namespace ed
