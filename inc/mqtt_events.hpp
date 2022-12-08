#pragma once

#include <mqtt/client.h>

namespace events {

struct connected_ev
{
    std::string cause;
};
struct connection_lost_ev
{
    std::string cause;
};
struct message_arrived_ev
{
    std::string topic, payload;
};
struct delivered_ev
{
    mqtt::delivery_token_ptr token;
};
}  // namespace events
