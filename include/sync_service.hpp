#pragma once

#include "message.hpp"
#include "mqtt_connector.hpp"

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

static size_t get_id()
{
    static std::atomic<size_t> _id{0};
    return _id++;
}

namespace sync {

template<common::Role role>
struct Service
{
    Service(const std::string service_name, const std::string subservice_name);
    ~Service();

    mqtt_connector::ResultType respond(message_serdes::Message req, std::string message);
    mqtt_connector::ResultType send(std::string message);
    Option<message_serdes::Message> receive();

  private:
    void on_connection_lost(std::string service_name, events::connection_lost_ev ev);
    void on_connected(std::string service_name, events::connected_ev ev);
    void on_message(std::string service_name, events::message_arrived_ev msg);

    std::queue<message_serdes::Message> message_queue;
    const std::string _service_name, _session_name;
    std::unique_ptr<mqtt_connector::Connector> connector_ptr;
};

template<common::Role role>
Service<role>::Service(const std::string service_name, const std::string subservice_name)
    : _service_name(get_connection_path(service_name, subservice_name)),
      _session_name(_service_name + random_string())
{
    CONNECT_MQTT_EV(_service_name, message_arrived_ev, Service::on_message);
    CONNECT_MQTT_EV(_service_name, connected_ev, Service::on_connected);
    CONNECT_MQTT_EV(_service_name, connection_lost_ev, Service::on_connection_lost);

    connector_ptr = std::make_unique<mqtt_connector::Connector>(_service_name, _session_name);
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
mqtt_connector::ResultType Service<role>::respond(message_serdes::Message req, std::string message)
{
    req.message = message;

    auto res = connector_ptr->publish(topic_manager::get_publish_topic(_service_name, role),
                                      message_serdes::to_json(req));
    if (res.is_value()) {
        res.get_value().unwrap()->wait();
    }
    return res;
}

template<common::Role role>
mqtt_connector::ResultType Service<role>::send(std::string message)
{
    message_serdes::Message msg_obj;

    msg_obj.message = message;
    msg_obj.message_id = get_id();
    msg_obj.client_id = _session_name;

    auto res = connector_ptr->publish(topic_manager::get_publish_topic(_service_name, role),
                                      message_serdes::to_json(msg_obj));
    if (res.is_value()) {
        res.get_value().unwrap()->wait();
    }
    return res;
}

template<common::Role role>
Option<message_serdes::Message> Service<role>::receive()
{
    Option<message_serdes::Message> res;
    if (message_queue.size()) {
        message_serdes::Message msg = message_queue.front();
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

        auto msg_deserialized = message_serdes::from_json<message_serdes::Message>(msg.payload);

        if (msg_deserialized.is_some()) {
            if (role == common::Role::Client) {
                if (msg_deserialized.unwrap().client_id != _session_name) {
                    return;
                }
            }

            message_queue.push(msg_deserialized.unwrap());
        }
    }
}

using Server = sync::Service<common::Role::Server>;
using Client = sync::Service<common::Role::Client>;

}  // namespace sync