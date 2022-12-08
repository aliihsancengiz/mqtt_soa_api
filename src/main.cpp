#include "mqtt_connector.hpp"

void new_message_arrived(events::message_arrived_ev msg)
{
    std::cout << msg.topic << " == " << msg.payload << std::endl;
}

int main(int argc, char* argv[])
{

    mqtt_connector::Connector cli("sample");
    cli.connect();
    cli.subscribe("/ali");
    event_bus::EventBus::getInstance().registerEvent<events::message_arrived_ev>(
      new_message_arrived);

    cli.publish("/ali", "12");
    cli.publish("/ali", "12");
    cli.publish("/ali", "12");
    cli.publish("/ali", "12");

    std::cout << "Hello World" << std::endl;
    const std::string service_name{"dio"};
    std::cout << topic_manager::get_request_topic(service_name) << std::endl;
    std::cout << topic_manager::get_response_topic(service_name) << std::endl;
    std::cout << topic_manager::get_heartbeat_topic(service_name) << std::endl;

    return 0;
}