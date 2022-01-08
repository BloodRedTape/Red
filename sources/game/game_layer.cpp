#include "game/game_layer.hpp"
#include <core/ranges.hpp>
#include <core/print.hpp>
#include <core/math/trig.hpp>
#include <cmath>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <graphics/api/gpu.hpp>

Vector2f MakeVec2(b2Vec2 in){
    return {in.x, in.y};
}

void ApplyForceContext::OnMouseDown(Vector2s position){

    for(auto body = World->GetBodyList(); body != nullptr; body = body->GetNext()){
        for(auto fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()){
            if(fixture->TestPoint({float(position.x), float(position.y)})){
                HoveredBody = body;
                BeginPosition = position;
                break;
            }
        }
    }
}

void ApplyForceContext::OnMouseUp(Vector2s position){
    if(!HoveredBody)return;

    Println("Applied");


    auto vec = Vector2f(position - BeginPosition);
    auto force = HoveredBody->GetMass() * vec;


    b2Vec2 begin = {(float)BeginPosition.x, (float)BeginPosition.y};
    HoveredBody->ApplyLinearImpulse({force.x, force.y}, begin, true);

    HoveredBody = nullptr;
}

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

GameLayer::GameLayer(const RenderPass *pass):
    m_RectRenderer(pass)
{
    m_Background = Texture2D::Create("resources/backgrounds/Classic.png");

    m_BlockTextures.Add(Texture2D::Create("resources/blocks/StoneEmpty.png"));
    m_BlockTextures.Add(Texture2D::Create("resources/blocks/StoneFull.png"));
    m_BlockTextures.Add(Texture2D::Create("resources/blocks/StoneHalf.png"));

    m_BlockTextures.Add(Texture2D::Create("resources/blocks/WoodEmpty.png"));
    m_BlockTextures.Add(Texture2D::Create("resources/blocks/WoodFull.png"));
    m_BlockTextures.Add(Texture2D::Create("resources/blocks/WoodHalf.png"));

    m_BlockTextures.Add(Texture2D::Create("resources/blocks/tntBig.png"));
    m_BlockTextures.Add(Texture2D::Create("resources/blocks/tntMedium.png"));
    m_BlockTextures.Add(Texture2D::Create("resources/blocks/tntSmall.png"));


    AddGameBlock({1280/2, 720 - 98}, nullptr, false);

    AddGameBlock({200, 200}, m_BlockTextures[6]);

    for(int i = 0; i<20; i++){
        AddGameBlock({rand()%1280, rand()%500}, m_BlockTextures[rand()%m_BlockTextures.Size()]);
    }
}


void GameLayer::Tick(float dt){
    Time time = Clock::GetMonotonicTime();

    if(m_PhysicsClock.GetElapsedTime().AsSeconds() >= 1.f/60){
        m_PhysicsClock.Restart();
        m_World.Step(1.f/60, 6, 2);
    }
}

void GameLayer::Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal){

    m_RectRenderer.BeginDrawing(wait, fb);

    m_RectRenderer.DrawRect({0,0}, Vector2s(fb->Size()), Color::White, m_Background);

    m_RectRenderer.Flush();

    for(auto[it, index]: IndexedRange(m_Blocks)){
        Block &block = *it;

        if(block.Texture)
            m_RectRenderer.DrawRect(MakeVec2(block.Body->GetPosition()), block.Size, block.Size/2, Math::Deg(block.Body->GetAngle()), Color::White, block.Texture);
        else
            m_RectRenderer.DrawRect(MakeVec2(block.Body->GetPosition()), block.Size, Math::Deg(block.Body->GetAngle()), Color::Transparent);
    }

    m_RectRenderer.EndDrawing(signal);
}

bool GameLayer::HandleEvent(const Event &e){

    if(e.Type == EventType::MouseButtonPress)
        m_Ctx.OnMouseDown({e.MouseButtonPress.x, e.MouseButtonPress.y});
    if(e.Type == EventType::MouseButtonRelease)
        m_Ctx.OnMouseUp({e.MouseButtonRelease.x, e.MouseButtonRelease.y});
    return false;
}

void GameLayer::AddGameBlock(Vector2s position, Texture2D *texture, bool is_dynamic){
    b2BodyDef def;
    def.position = b2Vec2(position.x, position.y);
    def.type = is_dynamic ? b2_dynamicBody : b2_staticBody;

    b2Body *body = m_World.CreateBody(&def);
    

    Vector2s size;
    if(texture)
        size = texture->Size();
    else
        size = {1280, 80};

    b2PolygonShape box;
    box.SetAsBox(size.x/2.f, size.y/2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    body->SetFixedRotation(false);

    Block block;
    block.Body = body;
    block.Texture = texture;
    block.Size = size;

    m_Blocks.Add(block);
}