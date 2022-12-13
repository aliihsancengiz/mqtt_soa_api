#pragma once

#include "nlohman/json.hpp"
#include "option.hpp"

#include <string>

namespace message_serdes {
struct Message
{
    size_t message_id;
    std::string client_id;
    std::string message;
};

template<typename Obj>
Option<Obj> from_json(const std::string& json_str);

template<typename Obj>
std::string to_json(const Obj& obj);

template<>
Option<Message> from_json<Message>(const std::string& json_str)
{
    nlohmann::json json_obj = nlohmann::json::parse(json_str);
    Option<Message> ret;
    Message m;
    try {

        m.message_id = json_obj.at("message_id").get<decltype(m.message_id)>();
        m.client_id = json_obj.at("client_id").get<decltype(m.client_id)>();
        m.message = json_obj.at("message").get<decltype(m.message)>();
    } catch (const std::exception& e) {
        return m;
    }
    ret.set_some(m);
    return ret;
}

template<>
std::string to_json<Message>(const Message& obj)
{
    nlohmann::json json_obj;
    json_obj["message_id"] = obj.message_id;
    json_obj["client_id"] = obj.client_id;
    json_obj["message"] = obj.message;

    return json_obj.dump();
}

}  // namespace message_serdes
