#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace ed
{

using Color = glm::vec4;

class Image
{
public:
    enum ImageType
    {
        eRaw,
        eJpg,
        ePng
    };

public:
    Image(uint32_t width, uint32_t height);
    Image(uint32_t width, uint32_t height, std::vector<Color> data);
    Image(const char* filepath);

    void load(const char* filepath);
    void store(const char* filepath, ImageType type);

    uint32_t width() const;
    uint32_t height() const;

    std::vector<Color>& data();

    void clear(Color color);

    Color getTexel(uint32_t x, uint32_t y) const;
    void  setTexel(uint32_t x, uint32_t y, Color color);

private:
    uint32_t mWidth;
    uint32_t mHeight;

    std::vector<Color> mData;
};
} // namespace ed