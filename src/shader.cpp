#include "shader.h"

#include "vertex.h"

namespace ed
{

void SimpleShader::vertex(const Triangle& triangle)
{
    for (size_t i = 0; i < 3; ++i) {
        const Vertex& v = triangle[i];

        vUV[i] = v.uv;

        glm::vec4 normal = *MVP * glm::vec4(v.normal.x, v.normal.y, v.normal.z, 1.0);
        vNormal[i] = normal.xyz();

        clipCoordinates[i] = *MVP * glm::vec4(v.position.x, v.position.y, v.position.z, 1.0);
        ndcCoordinates[i]  = clipCoordinates[i] / clipCoordinates[i].w;
    }
}

glm::vec4 SimpleShader::fragment(const glm::vec3& barycentric)
{
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 uv     = glm::vec2(0.0f, 0.0f);

    for (size_t i = 0; i < 3; ++i) {
        normal += vNormal[i] * barycentric[i];
        uv     += vUV[i] * barycentric[i];
    }

    normal = glm::normalize(normal);

    (void)(normal);

    const ColorRGBA& texel = texture->getTexel(uv.x * (texture->width() - 1), uv.y * (texture->height() - 1));
    return texel;
}

} // namespace ed
