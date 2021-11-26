#ifndef STRAITXSANDBOX_HELPERS_HPP
#define STRAITXSANDBOX_HELPERS_HPP

#include <memory>
#include "core/list.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/api/framebuffer.hpp"

using FramebufferRef = std::unique_ptr<Framebuffer>;

class FramebufferChain{
private:
    Swapchain *m_Swapchain = nullptr;
    RenderPass *m_SwapchainPass = nullptr;
    List<FramebufferRef> m_Framebuffers;
public:
    FramebufferChain(const Window *window);

    ~FramebufferChain();

    void Recreate();

    Framebuffer *CurrentFramebuffer()const{
        return m_Framebuffers[m_Swapchain->Current()].get();
    }

    RenderPass *Pass()const{
        return m_SwapchainPass;
    }

    void AcquireNext(const Semaphore *signal_semaphore){
        m_Swapchain->AcquireNext(*signal_semaphore);
    }

    void PresentCurrent(const Semaphore *wait_semaphore){
        m_Swapchain->PresentCurrent(*wait_semaphore);
    }
};

#endif //STRAITXSANDBOX_HELPERS_HPP