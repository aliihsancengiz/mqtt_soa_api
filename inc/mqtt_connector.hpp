#pragma once

#include "common.hpp"
#include "event_bus.hpp"
#include "mqtt_events.hpp"
#include "option.hpp"
#include "topic_manager.hpp"

namespace mqtt_connector {
using ResultType = Result<mqtt::token_ptr, common::MqttError>;
using OptionType = Option<mqtt::token_ptr>;

class Connector : mqtt::callback
{
  public:
    Connector(const std::string& _service_name);

    ResultType connect() noexcept;
    OptionType disconnect() noexcept;
    ResultType reconnect() noexcept;

    ResultType publish(const std::string& topic, const std::string& message);
    ResultType subscribe(const std::string& topic);

  private:
    virtual void connected(const std::string& cause) override;
    virtual void connection_lost(const std::string& cause) override;
    virtual void message_arrived(mqtt::const_message_ptr msg) override;
    virtual void delivery_complete(mqtt::delivery_token_ptr tok) override;

  private:
    std::string service_name;
    std::unique_ptr<mqtt::async_client> client_ptr;
    mqtt::connect_options conn_opts;
};

}  // namespace mqtt_connector