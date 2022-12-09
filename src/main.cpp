#include "mqtt_connector.hpp"

void new_message_arrived(std::string service_name, events::message_arrived_ev msg)
{
    std::cout << msg.topic << " == " << msg.payload << std::endl;
}

int main(int argc, char* argv[])
{

    mqtt_connector::Connector cli("sample");
    cli.connect();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cli.subscribe("/ali");
    event_bus::EventBus::getInstance().registerEvent<events::message_arrived_ev>(
      "sample", new_message_arrived);

    cli.publish("/ali", "12");
    cli.publish("/ali", "12");
    cli.publish("/ali", "12");
    cli.publish("/ali", "12");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cli.disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}