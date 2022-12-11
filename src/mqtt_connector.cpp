#include "mqtt_connector.hpp"

namespace mqtt_connector {

static std::string random_string()
{
    srand(time(NULL));  // seed with time

    auto randchar = []() -> char {
        const char charset[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    int length = rand() % 25;
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

Connector::Connector(const std::string& _service_name) : service_name(_service_name)
{
    session_name = service_name + random_string();
    client_ptr = std::make_unique<mqtt::async_client>("localhost", session_name, nullptr);
    client_ptr->set_callback(*this);
    conn_opts.set_clean_session(true);
}

ResultType Connector::connect() noexcept
{
    ResultType ret;
    if (!client_ptr) {
        client_ptr = std::make_unique<mqtt::async_client>("localhost", session_name, nullptr);
    }
    try {
        ret.set_value(client_ptr->connect(conn_opts));
    } catch (const std::exception& e) {
        ret.set_error(common::MqttError{e.what(), common::ReturnValue::CONNECTION_ERROR});
    }
    return ret;
}

OptionType Connector::disconnect() noexcept
{
    OptionType ret;
    if (client_ptr) {
        try {
            ret.set_some(client_ptr->disconnect());
        } catch (const std::exception& e) {
            ret.reset();
        }
    } else {
        ret.reset();
    }
    return ret;
}

ResultType Connector::reconnect() noexcept
{
    ResultType ret;
    if (!client_ptr) {
        client_ptr = std::make_unique<mqtt::async_client>("localhost", session_name, nullptr);
    }
    try {
        ret.set_value(client_ptr->reconnect());
    } catch (const std::exception& e) {
        ret.set_error(common::MqttError{e.what(), common::ReturnValue::RECONNECT_ERROR});
    }
    return ret;
}

ResultType Connector::publish(const std::string& topic, const std::string& message)
{
    ResultType ret;
    auto pubmsg = mqtt::make_message(topic, message);
    pubmsg->set_qos(1);
    if (client_ptr && client_ptr->is_connected()) {
        try {
            ret.set_value(client_ptr->publish(pubmsg));
        } catch (const std::exception& e) {
            ret.set_error({e.what(), common::ReturnValue::PUBLISH_ERROR});
        }
    } else {
        ret.set_error(
          {"Client is not initialized or connected.", common::ReturnValue::CONNECTION_ERROR});
    }
    return ret;
}

ResultType Connector::subscribe(const std::string& topic)
{
    ResultType ret;
    if (client_ptr && client_ptr->is_connected()) {
        try {
            ret.set_value(client_ptr->subscribe(topic, 1));
        } catch (const std::exception& e) {
            ret.set_error({e.what(), common::ReturnValue::SUBSCRIBE_ERROR});
        }
    } else {
        ret.set_error(
          {"Client is not initialized or connected.", common::ReturnValue::CONNECTION_ERROR});
    }
    return ret;
}

void Connector::connected(const std::string& cause)
{
    event_bus::EventBus::getInstance().fireEvent(service_name, events::connected_ev{cause});
}

void Connector::connection_lost(const std::string& cause)
{
    event_bus::EventBus::getInstance().fireEvent(service_name, events::connection_lost_ev{cause});
}

void Connector::message_arrived(mqtt::const_message_ptr msg)
{
    event_bus::EventBus::getInstance().fireEvent(
      service_name, events::message_arrived_ev{msg->get_topic(), msg->get_payload()});
}

void Connector::delivery_complete(mqtt::delivery_token_ptr tok)
{
    event_bus::EventBus::getInstance().fireEvent(service_name, events::delivered_ev{tok});
}

}  // namespace mqtt_connector