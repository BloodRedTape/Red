#ifndef GAME_LAYER_HPP
#define GAME_LAYER_HPP

#include <core/list.hpp>
#include "utils/layers.hpp"
#include "2d/rect_renderer.hpp"

struct Rect{
    Vector2f Position;
    Vector2s Size;
    Color    Tint;
};

class GameLayer: public Layer{
private:
    RectRenderer m_Renderer;

    List<Rect> m_Objects;
public:
    GameLayer(const RenderPass *pass);

    ~GameLayer();

    void Tick(float dt)override;

    void Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal)override;

    bool HandleEvent(const Event &e)override;
};

#endif//GAME_LAYER_HPP