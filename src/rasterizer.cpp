#include "rasterizer.h"

namespace ed
{

bool isZero(float value)
{
    return (std::abs(value) < 1e-6f) ? true : false;
}

} // namespace ed
