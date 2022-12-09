#pragma once

#include "mqtt_events.hpp"
#include "result.hpp"

#include <string>

#define CONNECT_MQTT_EV(svname, ev, handler)                              \
    event_bus::EventBus::getInstance().registerEvent<events::ev>( \
      svname, std::bind(&handler, this, std::placeholders::_1, std::placeholders::_2));

namespace common {

enum class Role
{
    Server,
    Client
};

enum class ReturnValue
{
    CONNECTION_ERROR,
    DISCONNECT_ERROR,
    RECONNECT_ERROR,
    PUBLISH_ERROR,
    SUBSCRIBE_ERROR
};

struct MqttError : ErrorBase
{
    std::string cause;
    ReturnValue err;
    MqttError(const std::string _cause, ReturnValue rc) : cause(_cause), err(rc) {}
    virtual std::string describe_what() override
    {
        return cause;
    }
};

#define get_connection_path(service, subservice) (service + "_" + subservice)

}  // namespace common