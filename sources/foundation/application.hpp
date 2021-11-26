#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <core/os/window.hpp>
#include <core/os/clock.hpp>

class Application{
private:
    Window m_MainWindow;
private:
    Application();
public:
    int Run();

    void Tick(float dt);

    void OnEvent(const Event &e);

    void Stop();

    static Application &Get();
};

#endif//APPLICATION_HPP