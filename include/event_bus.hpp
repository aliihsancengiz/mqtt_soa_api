#pragma once

#include <any>
#include <functional>
#include <future>
#include <map>
#include <typeindex>
#include <unordered_map>

namespace event_bus {

struct EventBus
{
    using eventHandler = std::function<void(std::any)>;

    static EventBus& getInstance()
    {
        static EventBus bus;
        return bus;
    }

    template<typename Event, typename Handler>
    void registerEvent(const std::string& topic, Handler&& handler)
    {
        const auto type_idx = std::type_index(typeid(Event));
        handlerRegister[topic].emplace(type_idx,
                                       [topic, func = std::forward<Handler>(handler)](auto value) {
                                           func(topic, std::any_cast<Event>(value));
                                       });
    }

    template<typename Event, typename Handler, typename... Rest>
    void registerEvent(const std::string& topic, Handler&& handler, Rest&&... rest)
    {
        const auto type_idx = std::type_index(typeid(Event));
        auto pck = std::bind(std::forward<Handler>(handler), std::placeholders::_1,
                             std::placeholders::_2, std::forward<Rest>(rest)...);

        handlerRegister[topic].emplace(type_idx, [topic, func = pck](auto value) {
            func(topic, std::any_cast<Event>(value));
        });
    }
    template<typename Event>
    void removeEvent(const std::string& topic)
    {
        handlerRegister[topic].erase(std::type_index(typeid(Event)));
    }
    template<typename Event>
    void fireEvent(const std::string& topic, Event ev)
    {
        auto [beginEventIter, endEventIter] =
          handlerRegister[topic].equal_range(std::type_index(typeid(Event)));
        for (; beginEventIter != endEventIter; ++beginEventIter) {
            beginEventIter->second(std::forward<Event>(ev));
        }
    }

  private:
    EventBus() {}
    EventBus(EventBus&&) = delete;
    EventBus(const EventBus&) = delete;
    using handlerRegisterType = std::unordered_multimap<std::type_index, eventHandler>;

    std::map<std::string, handlerRegisterType> handlerRegister;
};
}  // namespace event_bus