#pragma once

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include <stb_image.h>
#include <stb_image_write.h>

#include <vector>

namespace ed
{

using ColorR    = float;
using ColorRGB  = glm::vec3;
using ColorRGBA = glm::vec4;

enum ImageType
{
    eJpg,
    ePng
};

template <typename ColorT>
class Image
{
public:
    Image(uint32_t width, uint32_t height);
    Image(uint32_t width, uint32_t height, const std::vector<ColorT>& data);
    Image(const char* filepath);

    void load(const char* filepath);
    void store(const char* filepath, ImageType type);

    uint32_t width() const;
    uint32_t height() const;

    std::vector<ColorT>& data();

    void clear(const ColorT& color);

    ColorT getTexel(uint32_t x, uint32_t y) const;
    void  setTexel(uint32_t x, uint32_t y, const ColorT& color);

private:
    void computeChannels();

private:
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mChannels;

    std::vector<ColorT> mData;
};

//------------------------------------------------------------------------------------------------

template <typename ColorT>
Image<ColorT>::Image(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
{
    mData = std::vector<ColorT>(width * height);
    computeChannels();
}

template <typename ColorT>
Image<ColorT>::Image(uint32_t width, uint32_t height, const std::vector<ColorT>& data)
    : mWidth(width)
    , mHeight(height)
    , mData(data)
{
    computeChannels();
}

template <typename ColorT>
Image<ColorT>::Image(const char* filepath)
{
    computeChannels();
    load(filepath);
}

template <typename ColorT>
void Image<ColorT>::load(const char* filepath)
{
    int width;
    int height;
    int channels;

    int desiredChannels = STBI_default;
    if (mChannels == 1) {
        desiredChannels = STBI_grey;
    } else if (mChannels == 3) {
        desiredChannels = STBI_rgb;
    } else if (mChannels == 4) {
        desiredChannels = STBI_rgb_alpha;
    }

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, desiredChannels);

    mData.reserve(width * height);
    for (size_t j = 0; j < height; ++j) {
        for (size_t i = 0; i < width * mChannels; i += mChannels) {
            ColorT& color = mData[(j * width) + (i / mChannels)];
            const size_t line = j * width * mChannels;
            color[0] = static_cast<float>(data[line + i + 0]);

            if (mChannels < 3)
                continue;

            color[1] = static_cast<float>(data[line + i + 1]);
            color[2] = static_cast<float>(data[line + i + 2]);

            if (mChannels < 4)
                continue;

            color[3] = static_cast<float>(data[line + i + 3]);
        }
    }

    mWidth  = static_cast<uint32_t>(width);
    mHeight = static_cast<uint32_t>(height);

    stbi_image_free(data);
}

template <typename ColorT>
void Image<ColorT>::store(const char* filepath, ImageType type)
{
    unsigned char* data = (unsigned char*)malloc(mWidth * mHeight * mChannels * sizeof(unsigned char));

    for (size_t j = 0; j < mHeight; ++j) {
        for (size_t i = 0; i < mWidth * mChannels; i += mChannels) {
            ColorT& color = mData[(j * mWidth) + (i / mChannels)];
            const size_t line = j * mWidth * mChannels;
            data[line + i + 0] = static_cast<unsigned char>(color[0]);

            if (mChannels < 3)
                continue;

            data[line + i + 1] = static_cast<unsigned char>(color[1]);
            data[line + i + 2] = static_cast<unsigned char>(color[2]);

            if (mChannels < 4)
                continue;

            data[line + i + 3] = static_cast<unsigned char>(color[3]);
        }
    }

    stbi_flip_vertically_on_write(true);
    switch (type) {
        case ImageType::eJpg:
        {
            stbi_write_jpg(filepath, (int)mWidth, (int)mHeight, mChannels, data, 100);
            break;
        }
        default:
        case ImageType::ePng:
        {
            stbi_write_png(filepath, (int)mWidth, (int)mHeight, mChannels, data, mWidth * mChannels);
            break;
        }
    }

    free(data);
}

template <typename ColorT>
uint32_t Image<ColorT>::width() const
{
    return mWidth;
}

template <typename ColorT>
uint32_t Image<ColorT>::height() const
{
    return mHeight;
}

template <typename ColorT>
std::vector<ColorT>& Image<ColorT>::data()
{
    return mData;
}

template <typename ColorT>
void Image<ColorT>::clear(const ColorT& color)
{
    std::fill(mData.begin(), mData.end(), color);
}

template <typename ColorT>
ColorT Image<ColorT>::getTexel(uint32_t x, uint32_t y) const
{
    return mData[y * mWidth + x];
}

template <typename ColorT>
void  Image<ColorT>::setTexel(uint32_t x, uint32_t y, const ColorT& color)
{
    mData[y * mWidth + x] = color;
}

template <typename ColorT>
void Image<ColorT>::computeChannels()
{
    if (typeid(ColorT) == typeid(ColorR)) {
        mChannels = 1;
    } else if (typeid(ColorT) == typeid(ColorRGB)) {
        mChannels = 3;
    } else if (typeid(ColorT) == typeid(ColorRGBA)) {
        mChannels = 4;
    }
}

//------------------------------------------------------------------------------------------------

template <>
inline void Image<ColorR>::load(const char* filepath)
{
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, STBI_grey);

    mData.reserve(width * height);
    for (size_t j = 0; j < height; ++j) {
        for (size_t i = 0; i < width; ++i) {
            ColorR& color = mData[(j * width) + i];
            const size_t line = j * width;
            color = static_cast<float>(data[line + i]);
        }
    }

    mWidth  = static_cast<uint32_t>(width);
    mHeight = static_cast<uint32_t>(height);

    stbi_image_free(data);
}

template <>
inline void Image<ColorR>::store(const char* filepath, ImageType type)
{
    unsigned char* data = (unsigned char*)malloc(mWidth * mHeight * sizeof(unsigned char));

    for (size_t j = 0; j < mHeight; ++j) {
        for (size_t i = 0; i < mWidth; ++i) {
            ColorR& color = mData[(j * mWidth) + i];
            const size_t line = j * mWidth;
            data[line + i + 0] = static_cast<unsigned char>(color);
        }
    }

    stbi_flip_vertically_on_write(true);
    switch (type) {
        case ImageType::eJpg:
        {
            stbi_write_jpg(filepath, (int)mWidth, (int)mHeight, 1, data, 100);
            break;
        }
        default:
        case ImageType::ePng:
        {
            stbi_write_png(filepath, (int)mWidth, (int)mHeight, 1, data, mWidth);
            break;
        }
    }

    free(data);
}

} // namespace ed
