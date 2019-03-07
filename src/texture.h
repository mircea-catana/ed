#pragma once

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <cinttypes>
#include <vector>

namespace ed
{
    enum TextureType
    {
        eRGB  = 0u,
        eRGBA = 1u,
        eDS   = 2u
    };

    class Texture
    {
    public:
        Texture()                     = delete;
        Texture(const Texture& other) = delete;
        Texture(Texture&& other)      = delete;
        ~Texture()                    = default;
        Texture(TextureType type, uint32_t width, uint32_t height);

        Texture& operator= (const Texture& other) = delete;
        Texture& operator= (Texture&& other)      = delete;

        uint32_t width()  const;
        uint32_t height() const;

        void fill(uint8_t value);

        std::vector<uint8_t>& data();

    private:
        TextureType mType;
        uint32_t    mWidth;
        uint32_t    mHeight;

        std::vector<uint8_t> mData;
    };
}

#endif
