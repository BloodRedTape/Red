#include "game/game_layer.hpp"
#include <core/os/clock.hpp>
#include <cmath>
#include <random>


Color NiceRandomColor(){
    int i = rand() % 3;
    switch(i){
    case 0:
        return Color(byte(rand()%255), 50, 235);
    case 1:
        return Color(235, byte(rand()%255), 50);
    case 2:
        return Color(50, 235, byte(rand()%255));
    }
    return Color::White;
}

GameLayer::GameLayer(const RenderPass *pass){
    m_Renderer.Initialize(pass);

    for(int i = 0; i<40; i++){
        m_Objects.Add({{rand() % 1280, rand() % 720}, {rand() % 200, rand()%200}, NiceRandomColor()});
    }
}

GameLayer::~GameLayer(){
    m_Renderer.Finalize();
}

void GameLayer::Tick(float dt){
    Time time = Clock::GetMonotonicTime();

    for(auto &object: m_Objects){
        object.Position.x += sin(time.AsSeconds()) * object.Size.x * dt;
        object.Position.y += sin(time.AsSeconds() + Seconds(0.2).AsSeconds()) * object.Size.y * dt;

        object.Tint.R += sin(time.AsSeconds()) * dt;
        object.Tint.R += sin(time.AsSeconds()) * dt;
    }
}

void GameLayer::Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal){
    m_Renderer.BeginDrawing(wait, fb);

    m_Renderer.DrawRect({0,0}, Vector2s(fb->Size()), Color::Black);

    for(const auto &object: m_Objects)
        m_Renderer.DrawRect(Vector2s(object.Position), object.Size, object.Tint);

    m_Renderer.EndDrawing(signal);
}

bool GameLayer::HandleEvent(const Event &e){
    return false;
}