#ifndef LAYERS_HPP
#define LAYERS_HPP

#include <memory>
#include <core/os/events.hpp>
#include <core/type_traits.hpp>
#include <graphics/api/framebuffer.hpp>
#include <graphics/api/semaphore.hpp>

struct Layer{
    virtual ~Layer() = default;

    virtual void Tick(float dt) = 0;

    virtual void Draw(const Framebuffer *fb, const Semaphore *wait, const Semaphore *signal) = 0;

    virtual bool HandleEvent(const Event &e) = 0;
};

using LayerRef = std::unique_ptr<Layer>;

template<typename T, typename ...ArgsType>
inline LayerRef NewLayer(ArgsType&&...args){
    static_assert(IsBaseOf<Layer, T>::Value, "T is not a Layer");
    return std::make_unique<T>(Forward<ArgsType>(args)...);
}

#endif//LAYERS_HPP