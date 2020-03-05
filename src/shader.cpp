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
        vNormal[i]       = normal.xyz() / normal.w;

        clipCoordinates[i] = *MVP * glm::vec4(v.position.x, v.position.y, v.position.z, 1.0);
        ndcCoordinates[i]  = clipCoordinates[i].xyz() / clipCoordinates[i].w;
    }
}

glm::vec4 SimpleShader::fragment(const glm::vec3& barycentric)
{
    glm::vec3 normal   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 uv       = glm::vec2(0.0f, 0.0f);

    for (size_t i = 0; i < 3; ++i) {
        normal   += vNormal[i] * barycentric[i];
        uv       += vUV[i] * barycentric[i];
    }

    normal = glm::normalize(normal);

    glm::vec4 light = *MVP * glm::vec4(lightPosition.x, lightPosition.y, lightPosition.z, 1.0);
    glm::vec3 lightDirection = glm::normalize(light.xyz()/light.w);

    float intensity = 0.4f + 0.6f * std::max(glm::dot(normal, lightDirection), 0.0f);

    ColorRGBA texel = texture->getTexel(uv.x * (texture->width() - 1), uv.y * (texture->height() - 1));
    texel = ColorRGBA(texel.r * intensity, texel.g * intensity, texel.b * intensity, texel.a);
    return texel;
}

} // namespace ed
