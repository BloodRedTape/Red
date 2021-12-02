#ifndef GAME_LAYER_HPP
#define GAME_LAYER_HPP

#include <core/list.hpp>
#include <core/os/clock.hpp>
#include "utils/layers.hpp"
#include "2d/rect_renderer.hpp"
#include "2d/circle_renderer.hpp"
#include "2d/line_renderer.hpp"
#include <box2d/b2_world.h>

struct Rect{
    Vector2f Position;
    Vector2s Size;
    Color    Tint;
};

struct ApplyForceContext{
    b2World *World = nullptr;
    b2Body *HoveredBody = nullptr;
    Vector2s BeginPosition;

    void OnMouseDown(Vector2s position);

    void OnMouseUp(Vector2s position);
};

class GameLayer: public Layer{
private:
    RectRenderer m_RectRenderer;
    CircleRenderer m_CircleRenderer;
    LineRenderer m_LineRenderer;

    Semaphore m_BeginFrame[3];
    Semaphore m_EndFrame[3];

    List<Rect> m_Objects;
    List<b2Body *> m_Bodies;

    b2Vec2 m_Gravity{0, 100.f};
    b2World m_World{m_Gravity};

    Clock m_PhysicsClock;

    ApplyForceContext m_Ctx{&m_World, nullptr};
public:
    GameLayer(const RenderPass *pass);

    ~GameLayer() = default;

    void Tick(float dt)override;

    void Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal)override;

    bool HandleEvent(const Event &e)override;

private:
    void AddObject(Vector2s position, Vector2s size, Color color, bool is_dynamic = true);
};

#endif//GAME_LAYER_HPP