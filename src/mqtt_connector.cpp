#include "mqtt_connector.hpp"

namespace mqtt_connector {

Connector::Connector(const std::string& _service_name) : service_name(_service_name)
{
    client_ptr = std::make_unique<mqtt::client>("localhost", service_name, nullptr);
    client_ptr->set_callback(*this);
    conn_opts.set_clean_session(true);
}

void Connector::connect()
{
    if (!client_ptr) {
        client_ptr = std::make_unique<mqtt::client>("localhost", service_name, nullptr);
    }
    client_ptr->connect(conn_opts);
}

void Connector::disconnect()
{
    if (client_ptr) {
        client_ptr->disconnect();
    }
}

void Connector::reconnect()
{
    if (client_ptr) {
        client_ptr->reconnect();
    }
}

void Connector::publish(const std::string& topic, const std::string& message)
{
    auto pubmsg = mqtt::make_message(topic, message);
    pubmsg->set_qos(1);
    if (client_ptr) {
        client_ptr->publish(pubmsg);
    }
}

void Connector::subscribe(const std::string& topic)
{
    if (client_ptr) {
        client_ptr->subscribe(topic);
    }
}

void Connector::connected(const std::string& cause)
{
    event_bus::EventBus::getInstance().fireEvent(events::connected_ev{cause});
}

void Connector::connection_lost(const std::string& cause)
{
    event_bus::EventBus::getInstance().fireEvent(events::connection_lost_ev{cause});
}

void Connector::message_arrived(mqtt::const_message_ptr msg)
{
    event_bus::EventBus::getInstance().fireEvent(
      events::message_arrived_ev{msg->get_topic(), msg->get_payload()});
}

void Connector::delivery_complete(mqtt::delivery_token_ptr tok)
{
    event_bus::EventBus::getInstance().fireEvent(events::delivered_ev{tok});
}

}