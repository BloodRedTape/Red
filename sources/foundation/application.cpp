#include "foundation/application.hpp"
#include <core/print.hpp>
#include <core/ranges.hpp>
#include <imgui/backend.hpp>
#include "game/game_layer.hpp"
#include <cmath>
#include <core/os/sleep.hpp>

struct ImGuiLayer: public Layer{
    ImGuiBackend Back;

    ImGuiLayer(const RenderPass *pass){
        Back.Initialize(pass);
    }

    ~ImGuiLayer(){
        Back.Finalize();
    }

    void Tick(float dt){
        Back.NewFrame(dt, Mouse::RelativePosition(Application::Get().MainWindow()), Vector2s(Application::Get().MainWindow().Size()));
    }

    void Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal){
        Back.RenderFrame(fb, wait, signal);
    }

    bool HandleEvent(const Event &e){
        return false;
    }
};

Application::Application(){
    m_MainWindow.Open(1280, 720, "UtterCraft");

    Function<void(const Event&)> handler;
    handler.Bind<Application, &Application::OnEvent>(this);
    m_MainWindow.SetEventsHanlder(handler);

    m_Semaphores.Emplace();

    m_Swapchain = new FramebufferChain(&m_MainWindow);

    AddLayer(NewLayer<GameLayer>(m_Swapchain->Pass()));
    AddLayer(NewLayer<ImGuiLayer>(m_Swapchain->Pass()));
}

int Application::Run(){
    Time prev_frame_time = Clock::GetMonotonicTime();

    Clock cl;
    while(m_MainWindow.IsOpen()){
        Time current_time = Clock::GetMonotonicTime();

        float dt = (current_time - prev_frame_time).AsSeconds();
        prev_frame_time = current_time;

        dt = cl.GetElapsedTime().AsSeconds();
        cl.Restart();

        if(dt <= 0)
            dt = 0.0000001;
        
        Sleep(Seconds(std::max(0.f, 0.016f - dt)));

        m_MainWindow.DispatchEvents();

        Tick(dt);
    }

    return 0;
}

void Application::Tick(float dt){
    for(auto &layer: ReverseRange(m_Layers)){
        layer->Tick(dt);
    }

    m_Swapchain->AcquireNext(&m_Semaphores.First());

    for(auto i = 0; i<m_Layers.Size(); i++){
        auto &layer = m_Layers[i];
        layer->Draw(m_Swapchain->CurrentFramebuffer(), &m_Semaphores[i], &m_Semaphores[i + 1]);
    }

    m_Swapchain->PresentCurrent(&m_Semaphores.Last());
}

void Application::OnEvent(const Event &e){
    if(e.Type == EventType::WindowClose)
        return Stop();

    if(e.Type == EventType::WindowResized)
        m_Swapchain->Recreate();
    
    for(auto& layer: ReverseRange(m_Layers)){
        if(layer->HandleEvent(e))
            break;
    }
}

void Application::Stop(){
    delete m_Swapchain;
    m_Layers.Clear();
    m_MainWindow.Close();
}

void Application::AddLayer(LayerRef layer){
    m_Layers.Add(Move(layer));
    m_Semaphores.Emplace();
}

Application &Application::Get(){
    static Application app;
    return app;
}