#include "topic_manager.hpp"

#include <gtest/gtest.h>

const std::string sample_service = "sample";

TEST(TopicManager, GetRequestTopic)
{
    ASSERT_EQ(topic_manager::get_request_topic(sample_service),
              std::string("ipc/v1/sample/request"));
}

TEST(TopicManager, GetResponseTopic)
{
    ASSERT_EQ(topic_manager::get_response_topic(sample_service),
              std::string("ipc/v1/sample/response"));
}

TEST(TopicManager, GetHeartBearTopic)
{
    ASSERT_EQ(topic_manager::get_heartbeat_topic(sample_service),
              std::string("ipc/v1/sample/heartbeat"));
}

TEST(TopicManager, GetSubscribeTopic)
{
    ASSERT_EQ(topic_manager::get_subscribe_topic(sample_service, common::Role::Client),
              std::string("ipc/v1/sample/response"));

    ASSERT_EQ(topic_manager::get_subscribe_topic(sample_service, common::Role::Server),
              std::string("ipc/v1/sample/request"));
}

TEST(TopicManager, GetPublishTopic)
{
    ASSERT_EQ(topic_manager::get_publish_topic(sample_service, common::Role::Client),
              std::string("ipc/v1/sample/request"));

    ASSERT_EQ(topic_manager::get_publish_topic(sample_service, common::Role::Server),
              std::string("ipc/v1/sample/response"));
}