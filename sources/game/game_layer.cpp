#include "game/game_layer.hpp"
#include <core/os/clock.hpp>
#include <core/ranges.hpp>
#include <core/print.hpp>
#include <core/math/trig.hpp>
#include <cmath>
#include <random>
#include <box2d/b2_body.h>
#include <box2d/b2_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include <imgui/widgets.hpp>
#include "foundation/application.hpp"

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
    auto length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
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
    m_Renderer(pass)
{
#if 1
    for(int i = 0; i<10; i++){
        AddObject({rand() % 1280, rand() % 680}, {rand() % 200, rand()%200}, NiceRandomColor());
    }
#endif
    AddObject({0,680}, {1280, 40}, Color::Green, false);

    AddObject({300,300}, {100, 100}, Color::Green, true);
}


void GameLayer::Tick(float dt){
    Time time = Clock::GetMonotonicTime();

    if(m_PhysicsClock.GetElapsedTime().AsSeconds() >= 1.f/60){
        m_PhysicsClock.Restart();
        m_World.Step(1.f/60, 6, 2);
    }

    for(auto &object: m_Objects){
        //object.Position.x += sin(time.AsSeconds()) * object.Size.x * dt;
        //object.Position.y += sin(time.AsSeconds() + Seconds(0.2).AsSeconds()) * object.Size.y * dt;

        //object.Tint.R += sin(time.AsSeconds()) * dt;
        //object.Tint.R += sin(time.AsSeconds()) * dt;
    }
}

void GameLayer::Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal){
    m_Renderer.BeginDrawing(wait, fb);

    m_Renderer.DrawRect({0,0}, Vector2s(fb->Size()), Color::Black);

    static Vector2f position{200, 200};
    static Vector2f size{100, 100};
    static Vector2f origin = size/2.f;
    static float rotation;

    ImGui::Begin("Quad");
    ImGui::SliderFloat2("position", &position[0], 0, 720);
    ImGui::SliderFloat2("size", &size[0], -100, 100);
    ImGui::SliderFloat2("origin", &origin[0], 0, 100);
    ImGui::SliderFloat("angle", &rotation, -180, 180);
    ImGui::End();

    m_Renderer.DrawRect(Vector2s(position), Vector2s(size), Vector2s(origin), rotation, Color::Mint);

    //for(const auto &object: m_Objects)
     //   m_Renderer.DrawRect(Vector2s(object.Position), object.Size, object.Tint);

    for(auto[it, index]: IndexedRange(m_Bodies)){
        b2Body &body = **it;
        auto pos = body.GetPosition();
        m_Renderer.DrawRect(Vector2s(pos.x, pos.y), m_Objects[index].Size, Math::Deg(body.GetAngle()), m_Objects[index].Tint);
    }

    m_Renderer.DrawRect(Mouse::RelativePosition(Application::Get().MainWindow()), {10, 10}, Color::Red);

    if(m_Ctx.HoveredBody){
        m_Renderer.DrawRect(m_Ctx.BeginPosition, {10, 10}, Color::White);
        m_Renderer.DrawRect(Mouse::RelativePosition(Application::Get().MainWindow()), {10, 10}, Color::White);
    }

    m_Renderer.EndDrawing(signal);
}

bool GameLayer::HandleEvent(const Event &e){

    if(e.Type == EventType::MouseButtonPress)
        m_Ctx.OnMouseDown({e.MouseButtonPress.x, e.MouseButtonPress.y});
    if(e.Type == EventType::MouseButtonRelease)
        m_Ctx.OnMouseUp({e.MouseButtonRelease.x, e.MouseButtonRelease.y});
    return false;
}

void GameLayer::AddObject(Vector2s position, Vector2s size, Color color, bool is_dynamic){
    m_Objects.Add({Vector2f(position), size, color});

    position.x += size.x/2.f;
    position.y += size.y/2.f;

    b2BodyDef def;
    def.position = b2Vec2(position.x, position.y);
    def.type = is_dynamic ? b2_dynamicBody : b2_staticBody;

    b2Body *body = m_World.CreateBody(&def);

    b2PolygonShape box;
    box.SetAsBox(size.x/2.f, size.y/2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    /*b2Fixture *fixture = */body->CreateFixture(&fixtureDef);
    body->SetFixedRotation(false);
    m_Bodies.Add(body);
    
}