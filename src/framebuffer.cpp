#include "framebuffer.h"

namespace ed
{

Framebuffer::Framebuffer()
    : mDepthAttachment(nullptr)
    , mStencilAttachment(nullptr)
{
    mColorAttachments.reserve(4);
}

Framebuffer::Framebuffer(ColorAttachmentT colorAttachment)
    : mDepthAttachment(nullptr)
    , mStencilAttachment(nullptr)
{
    mColorAttachments.reserve(4);
    mColorAttachments.push_back(colorAttachment);
}

Framebuffer::Framebuffer(ColorAttachmentT colorAttachment, DSAttachmentT depthAttachment)
    : mStencilAttachment(nullptr)
{
    mColorAttachments.reserve(4);
    mColorAttachments.push_back(colorAttachment);
    mDepthAttachment = depthAttachment;
}

Framebuffer::Framebuffer(ColorAttachmentT colorAttachment, DSAttachmentT depthAttachment, DSAttachmentT stencilAttachment)
{
    mColorAttachments.reserve(4);
    mColorAttachments.push_back(colorAttachment);
    mDepthAttachment   = depthAttachment;
    mStencilAttachment = stencilAttachment;
}


size_t Framebuffer::getNumColorAttachments() const
{
    return mColorAttachments.size();
}

ColorAttachmentT Framebuffer::getColorAttachment(size_t index)
{
    if (index >= mColorAttachments.size())
        return nullptr;

    return mColorAttachments[index];
}

void Framebuffer::addColorAttachment(ColorAttachmentT colorAttachment)
{
    mColorAttachments.push_back(colorAttachment);
}

bool Framebuffer::setColorAttachment(ColorAttachmentT colorAttachment, size_t index)
{
    if (index >= mColorAttachments.size())
        return false;

    mColorAttachments[index] = colorAttachment;
    return true;
}

DSAttachmentT Framebuffer::getDepthAttachment()
{
    return mDepthAttachment;
}

void Framebuffer::setDepthAttachment(DSAttachmentT depthAttachment)
{
    mDepthAttachment = depthAttachment;
}

DSAttachmentT Framebuffer::getStencilAttachment()
{
    return mStencilAttachment;
}

void Framebuffer::setStencilAttachment(DSAttachmentT stencilAttachment)
{
    mStencilAttachment = stencilAttachment;
}

} // namespace ed
