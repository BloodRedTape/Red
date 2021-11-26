#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <core/os/window.hpp>
#include <core/os/clock.hpp>
#include <core/list.hpp>

#include "utils/layers.hpp"
#include "utils/helpers.hpp"

class Application{
private:
    Window m_MainWindow;
    List<LayerRef> m_Layers;
    List<Semaphore> m_Semaphores;
    FramebufferChain *m_Swapchain = nullptr;
private:
    Application();
public:
    int Run();

    void Tick(float dt);

    void OnEvent(const Event &e);

    void Stop();

    void AddLayer(LayerRef layer);

    Window &MainWindow(){
        return m_MainWindow;
    }

    static Application &Get();
};

#endif//APPLICATION_HPP