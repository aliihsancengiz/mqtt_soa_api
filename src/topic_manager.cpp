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

}  // namespace topic_manager
