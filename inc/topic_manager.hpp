#pragma once

#include <string>

namespace topic_manager {

const std::string lib_version = "v1";
const std::string lib_prefix = "/ipc/";

const std::string request_path = "/request";
const std::string response_path = "/response";
const std::string heartbeat_path = "/heartbeat";

std::string get_request_topic(const std::string& service_name);
std::string get_response_topic(const std::string& service_name);
std::string get_heartbeat_topic(const std::string& service_name);

}  // namespace namespace_manager
