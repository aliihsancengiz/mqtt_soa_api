set(TARGET_NAME "mqtt-ipc-service")

set(TARGET_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/topic_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mqtt_connector.cpp
)

add_executable(${TARGET_NAME} ${TARGET_SOURCES})

target_include_directories(${TARGET_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
		${PROJECT_SOURCE_DIR}/event-bus
    PRIVATE
        ""
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
		paho-mqttpp3
		PahoMqttC::PahoMqttC
    PRIVATE
        ""
)

