#pragma once

#include "event_bus.hpp"
#include "mqtt_events.hpp"
#include "topic_manager.hpp"

namespace mqtt_connector {

class Connector : mqtt::callback
{
  public:
    Connector(const std::string& _service_name);

    void connect();
    void disconnect();
    void reconnect();

    void publish(const std::string& topic, const std::string& message);
    void subscribe(const std::string& topic);

  private:
    virtual void connected(const std::string& cause) override;
    virtual void connection_lost(const std::string& cause) override;
    virtual void message_arrived(mqtt::const_message_ptr msg) override;
    virtual void delivery_complete(mqtt::delivery_token_ptr tok) override;

  private:
    std::string service_name;
    std::unique_ptr<mqtt::client> client_ptr;
    mqtt::connect_options conn_opts;
};

}  // namespace mqtt_connector