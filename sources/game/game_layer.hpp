#ifndef GAME_LAYER_HPP
#define GAME_LAYER_HPP

#include <core/list.hpp>
#include <core/os/clock.hpp>
#include "utils/layers.hpp"
#include "2d/rect_renderer.hpp"
#include "2d/circle_renderer.hpp"
#include "2d/line_renderer.hpp"
#include <box2d/b2_world.h>

struct Block{
    Texture2D *Texture = nullptr;
    b2Body    *Body    = nullptr;
    Vector2s   Size;
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

    List<Block> m_Blocks;

    Texture2D *m_Background = nullptr;

    FixedList<Texture2D *, 9> m_BlockTextures;

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
    void AddGameBlock(Vector2s position, Texture2D *texture, bool is_dynamic = true);
};

#endif//GAME_LAYER_HPP