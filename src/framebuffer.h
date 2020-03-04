#pragma once

#include "image.h"

#include <memory>

namespace ed
{

using ColorAttachmentT = std::shared_ptr<Image<ColorRGBA>>;
using DSAttachmentT    = std::shared_ptr<Image<ColorR>>;

class Framebuffer
{
public:
    Framebuffer();
    Framebuffer(ColorAttachmentT colorAttachment);
    Framebuffer(ColorAttachmentT colorAttachment, DSAttachmentT depthAttachment);
    Framebuffer(ColorAttachmentT colorAttachment, DSAttachmentT depthAttachment, DSAttachmentT stencilAttachment);

    size_t getNumColorAttachments() const;

    ColorAttachmentT getColorAttachment(size_t index);
    void addColorAttachment(ColorAttachmentT colorAttachment);
    bool setColorAttachment(ColorAttachmentT colorAttachment, size_t index);

    DSAttachmentT getDepthAttachment();
    void setDepthAttachment(DSAttachmentT depthAttachment);

    DSAttachmentT getStencilAttachment();
    void setStencilAttachment(DSAttachmentT stencilAttachment);

private:
    std::vector<ColorAttachmentT>  mColorAttachments;
    DSAttachmentT                  mDepthAttachment;
    DSAttachmentT                  mStencilAttachment;
};
} // namespace ed
