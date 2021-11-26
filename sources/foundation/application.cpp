#include "foundation/application.hpp"
#include <core/print.hpp>

Application::Application(){
    m_MainWindow.Open(1280, 720, "UtterCraft");

    Function<void(const Event&)> handler;
    handler.Bind<Application, &Application::OnEvent>(this);
    m_MainWindow.SetEventsHanlder(handler);

}

int Application::Run(){
    Clock cl;
    while(m_MainWindow.IsOpen()){
        float dt = cl.GetElapsedTime().AsSeconds();
        cl.Restart();
        m_MainWindow.DispatchEvents();

        Tick(dt);
    }

    return 0;
}

void Application::Tick(float dt){

}

void Application::OnEvent(const Event &e){
    Println("OnEvent");

    if(e.Type == EventType::WindowClose)
        Stop();
}

void Application::Stop(){
    m_MainWindow.Close();
}

Application &Application::Get(){
    static Application app;
    return app;
}