#include "helpers.hpp"
#include "core/array.hpp"

FramebufferChain::FramebufferChain(const Window *window) {
    m_Swapchain = Swapchain::Create(window);

    Array<AttachmentDescription, 1> s_Attachments = {
        {
            TextureLayout::PresentSrcOptimal,
            TextureLayout::ColorAttachmentOptimal,
            TextureLayout::PresentSrcOptimal,
            m_Swapchain->Format(),
            SamplePoints::Samples_1
        }
    };

    m_SwapchainPass = RenderPass::Create({s_Attachments});

    Recreate();
}

FramebufferChain::~FramebufferChain() {
    delete m_SwapchainPass;
    delete m_Swapchain;
}

void FramebufferChain::Recreate() {
    m_Framebuffers.Clear();
    for(Texture2D *image: m_Swapchain->Images()){
        FramebufferProperties props;
        props.Pass = m_SwapchainPass;
        props.Size = m_Swapchain->Size();
        props.Attachments = {&image, 1};

        m_Framebuffers.Add(FramebufferRef(Framebuffer::Create(props)));
    }
}
