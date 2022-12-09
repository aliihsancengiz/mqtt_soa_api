#pragma once

#include "result.hpp"

#include <string>

namespace common {

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

}  // namespace common