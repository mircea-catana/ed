#pragma once

#include <array>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include "image.h"
#include "triangle.h"

namespace ed
{

struct Shader
{
    std::array<glm::vec4, 3> clipCoordinates;
    std::array<glm::vec3, 3> ndcCoordinates;

    virtual ~Shader() {}
    virtual void vertex(const Triangle& triangle) = 0;
    virtual glm::vec4 fragment(const glm::vec3& barycentric) = 0;
};


//------------------------------------------------------------------------------------------------

struct SimpleShader : public Shader
{
    // Vertex Shader Inputs
    glm::mat4* MVP;

    // Fragment Shader Inputs
    Image<ColorRGBA>* texture;

    // Shader Context (Varyings)
    std::array<glm::vec2, 3> vUV;
    std::array<glm::vec3, 3> vNormal;

    void vertex(const Triangle& triangle) override;
    glm::vec4 fragment(const glm::vec3& barycentric) override;

};

} // namespace ed
