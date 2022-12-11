set(TARGET_NAME "mqttsoa")

file(GLOB TARGET_SOURCES CONFIGURE_DEPENDS *.cpp )

add_library(${TARGET_NAME} SHARED ${TARGET_SOURCES})

target_include_directories(${TARGET_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ""
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        paho-mqttpp3
        PahoMqttC::PahoMqttC
    PRIVATE
)


