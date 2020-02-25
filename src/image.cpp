#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace ed
{

Image::Image(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
{
    mData = std::vector<Color>(width * height);
}

Image::Image(uint32_t width, uint32_t height, const std::vector<Color>& data)
    : mWidth(width)
    , mHeight(height)
    , mData(data)
{
}

Image::Image(const char* filepath)
{
    load(filepath);
}

void Image::load(const char* filepath)
{
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);

    mData.reserve(width * height);
    for (size_t j = 0; j < height; ++j) {
        for (size_t i = 0; i < width * 4; i += 4) {
            Color& color = mData[(j * width) + (i / 4)];
            const size_t line = j * width * 4;
            color.r = static_cast<float>(data[line + i + 0]);
            color.g = static_cast<float>(data[line + i + 1]);
            color.b = static_cast<float>(data[line + i + 2]);
            color.a = static_cast<float>(data[line + i + 3]);
        }
    }

    mWidth  = static_cast<uint32_t>(width);
    mHeight = static_cast<uint32_t>(height);

    stbi_image_free(data);
}

void Image::store(const char* filepath, ImageType type)
{
    unsigned char* data = (unsigned char*)malloc(mWidth * mHeight * 4 * sizeof(unsigned char));

    for (size_t j = 0; j < mHeight; ++j) {
        for (size_t i = 0; i < mWidth * 4; i += 4) {
            Color& color = mData[(j * mWidth) + (i / 4)];
            const size_t line = j * mWidth * 4;
            data[line + i + 0] = static_cast<unsigned char>(color.r);
            data[line + i + 1] = static_cast<unsigned char>(color.g);
            data[line + i + 2] = static_cast<unsigned char>(color.b);
            data[line + i + 3] = static_cast<unsigned char>(color.a);
        }
    }

    stbi_flip_vertically_on_write(true);
    switch (type) {
        case ImageType::eJpg:
        {
            stbi_write_jpg(filepath, (int)mWidth, (int)mHeight, 4, data, 100);
            break;
        }
        default:
        case ImageType::ePng:
        {
            stbi_write_png(filepath, (int)mWidth, (int)mHeight, 4, data, mWidth * 4);
            break;
        }
    }

    free(data);
}

uint32_t Image::width() const
{
    return mWidth;
}

uint32_t Image::height() const
{
    return mHeight;
}

std::vector<Color>& Image::data()
{
    return mData;
}

void Image::clear(const Color& color)
{
    std::fill(mData.begin(), mData.end(), color);
}

Color Image::getTexel(uint32_t x, uint32_t y) const
{
    return mData[y * mWidth + x];
}

void  Image::setTexel(uint32_t x, uint32_t y, const Color& color)
{
    mData[y * mWidth + x] = color;
}
} // namespace ed
