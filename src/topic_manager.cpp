#include "topic_manager.hpp"

namespace topic_manager {

std::string get_request_topic(const std::string& service_name)
{
    return lib_prefix + lib_version + "/" + service_name + request_path;
}
std::string get_response_topic(const std::string& service_name)
{
    return lib_prefix + lib_version + "/" + service_name + response_path;
}
std::string get_heartbeat_topic(const std::string& service_name)
{
    return lib_prefix + lib_version + "/" + service_name + heartbeat_path;
}

std::string get_subscribe_topic(const std::string& service_name, common::Role r)
{
    if (r == common::Role::Client) {
        return get_response_topic(service_name);
    } else {
        return get_request_topic(service_name);
    }
}

std::string get_publish_topic(const std::string& service_name, common::Role r)
{
    if (r == common::Role::Client) {
        return get_request_topic(service_name);
    } else {
        return get_response_topic(service_name);
    }
}

}  // namespace topic_manager
