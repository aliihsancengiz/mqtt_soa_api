#pragma once

#include "mqtt_connector.hpp"

namespace sync {

template<common::Role role>
struct Service
{
    Service(const std::string service_name, const std::string subservice_name);
    ~Service();

    mqtt_connector::ResultType send(std::string message);
    Option<std::string> receive();

  private:
    void on_connection_lost(std::string service_name, events::connection_lost_ev ev);
    void on_connected(std::string service_name, events::connected_ev ev);
    void on_message(std::string service_name, events::message_arrived_ev msg);

    std::queue<std::string> message_queue;
    const std::string _service_name;
    std::unique_ptr<mqtt_connector::Connector> connector_ptr;
};

template<common::Role role>
Service<role>::Service(const std::string service_name, const std::string subservice_name)
    : _service_name(get_connection_path(service_name, subservice_name))
{
    CONNECT_MQTT_EV(_service_name, message_arrived_ev, Service::on_message);
    CONNECT_MQTT_EV(_service_name, connected_ev, Service::on_connected);
    CONNECT_MQTT_EV(_service_name, connection_lost_ev, Service::on_connection_lost);

    connector_ptr = std::make_unique<mqtt_connector::Connector>(_service_name);
    auto res = connector_ptr->connect();
    if (res.is_error()) {
        throw std::runtime_error(res.get_error().unwrap().describe_what());
    } else {
        res.get_value().unwrap()->wait();
    }

    auto sub_res =
      connector_ptr->subscribe(topic_manager::get_subscribe_topic(_service_name, role));
    if (sub_res.is_value()) {
        sub_res.get_value().unwrap()->wait();
    } else {
        throw std::runtime_error(sub_res.get_error().unwrap().describe_what());
    }
}

template<common::Role role>
Service<role>::~Service()
{
    auto res = connector_ptr->disconnect();
    if (res.is_some()) {
        res.unwrap()->wait();
    } else {
        // TODO : handle unsuccessfull disconnect request
    }
}

template<common::Role role>
mqtt_connector::ResultType Service<role>::send(std::string message)
{
    auto res =
      connector_ptr->publish(topic_manager::get_publish_topic(_service_name, role), message);
    if (res.is_value()) {
        res.get_value().unwrap()->wait();
    }
    return res;
}

template<common::Role role>
Option<std::string> Service<role>::receive()
{
    Option<std::string> res;
    if (message_queue.size()) {
        std::string msg = message_queue.front();
        message_queue.pop();
        res.set_some(msg);
    }
    return res;
}

template<common::Role role>
void Service<role>::on_connection_lost(std::string service_name, events::connection_lost_ev ev)
{
    if (service_name == this->_service_name) {
        auto res = connector_ptr->reconnect();
        if (res.is_value()) {
            res.get_value().unwrap()->wait();
        } else {
            // TODO : Handle unsuccessfull reconnect attempt
        }
    }
}

template<common::Role role>
void Service<role>::on_connected(std::string service_name, events::connected_ev ev)
{
    if (service_name == this->_service_name) {
        std::cout << "Connected to Broker " << std::endl;
    }
}

template<common::Role role>
void Service<role>::on_message(std::string service_name, events::message_arrived_ev msg)
{
    if (service_name == this->_service_name &&
        msg.topic == topic_manager::get_subscribe_topic(_service_name, role)) {
        message_queue.push(msg.payload);
    }
}

using Server =sync::Service<common::Role::Server>;
using Client =sync::Service<common::Role::Client>;


}  // namespace sync