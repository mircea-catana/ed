#pragma once

#include "ed.h"
#include "vector.h"

namespace ed
{
    struct Color
    {
        Color() : r(0u), g(0u), b(0u), a(255u) {}
        Color(uint32_t r, uint32_t g, uint32_t b) : r(r), g(g), b(b), a(255u) {}
        Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a) : r(r), g(g), b(b), a(a) {}

        uint32_t r;
        uint32_t g;
        uint32_t b;
        uint32_t a;
    };

    class Image
    {
    public:
        Image(uint32_t width, uint32_t height);

        void fill(const Color& color);

        Color get(uint32_t x, uint32_t y) const;
        void  set(uint32_t x, uint32_t y, const Color& color);

        uint32_t width()  const;
        uint32_t height() const;

        void writePPM(const char* filepath, bool yFlip = true) const;
        
    private:
        uint32_t           mWidth;
        uint32_t           mHeight;
        std::vector<Color> mData;
    };

    class Framebuffer
    {
    public:
        Framebuffer(uint32_t width, uint32_t height);

        Image& output();

    private:
        Image mOutput;
    };

    Image::Image(uint32_t width, uint32_t height)
        : mWidth(width)
        , mHeight(height)
        , mData(width * height)
    {
    }

    void Image::fill(const Color& color)
    {
        std::fill(mData.begin(), mData.end(), color);
    }

    Color Image::get(uint32_t x, uint32_t y) const
    {
        return mData[y * mWidth + x];
    }

    void Image::set(uint32_t x, uint32_t y, const Color& color)
    {
        uint32_t index = y * mWidth + x;
        if (index >= mData.size())
            return;

        mData[index] = color;
    }

    uint32_t Image::width() const
    {
        return mWidth;
    }
    uint32_t Image::height() const
    {
        return mHeight;
    }

    void Image::writePPM(const char* filepath, bool yFlip) const
    {
        std::ofstream fileStream;
        fileStream.open(filepath);

        fileStream << "P3\n" << mWidth << " " << mHeight << "\n255\n";

        if (yFlip) {
            for (int32_t j = mHeight-1; j >= 0; --j) {
                const uint32_t jw = j * mWidth;
                for (uint32_t i = 0; i < mWidth; ++i) {
                    const uint32_t idx = jw + i;
                    fileStream << static_cast<int>(mData[idx].r) << " "
                               << static_cast<int>(mData[idx].g) << " "
                               << static_cast<int>(mData[idx].b) << "\n";
                }
            }
        } else {
            for (uint32_t j = 0; j < mHeight; ++j) {
                const uint32_t jw = j * mWidth;
                for (uint32_t i = 0; i < mWidth; ++i) {
                    const uint32_t idx = jw + i;
                    fileStream << static_cast<int>(mData[idx].r) << " "
                               << static_cast<int>(mData[idx].g) << " "
                               << static_cast<int>(mData[idx].b) << "\n";
                }
            }
        }

        fileStream.close();
    }

    Framebuffer::Framebuffer(uint32_t width, uint32_t height)
        : mOutput(width, height)
    {
    }

    Image& Framebuffer::output()
    {
        return mOutput;
    }
}
