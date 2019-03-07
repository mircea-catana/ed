#include "texture.h"

namespace ed
{
    static const uint32_t kNumComponents[] = {
        3u, // eRGB
        4u, // eRGBA
        1u  // eDS
    };

    Texture::Texture(TextureType type, uint32_t width, uint32_t height)
        : mType(type)
        , mWidth(width)
        , mHeight(height)
    {
        mData = std::vector<uint8_t>(mWidth * mHeight * kNumComponents[mType]);
    }

    uint32_t Texture::width() const
    {
        return mWidth;
    }

    uint32_t Texture::height() const
    {
        return mHeight;
    }

    void Texture::fill(uint8_t value)
    {
        const uint32_t size = mWidth * mHeight * kNumComponents[mType];
        for (uint32_t i = 0u; i < size; ++i) {
            mData[i] = value;
        }
    }

    std::vector<uint8_t>& Texture::data()
    {
        return mData;
    }
}
