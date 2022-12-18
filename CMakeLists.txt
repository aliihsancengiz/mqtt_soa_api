cmake_minimum_required(VERSION 3.15)

set(TARGET_NAME "mqttsoa")

project(mqtt-ipc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TEST "Build with test" OFF)

find_package(PahoMqttCpp)
add_subdirectory(src)

if(BUILD_TEST)
    add_subdirectory(test)
endif(BUILD_TEST)

add_subdirectory(examples)

set(TARGET_INC
    include/common.hpp
    include/event_bus.hpp
    include/mqtt_connector.hpp
    include/mqtt_events.hpp
    include/option.hpp
    include/result.hpp
    include/topic_manager.hpp
    include/service.hpp
    include/message.hpp
)

set(MODELS_CMAKE_FILES
    "cmake/mqttsoaConfig.cmake"
    "cmake/mqttsoaConfigVersion.cmake"
)

set(MODELS_LIBRARY_INSTALL_DIR "/usr/lib/")
set(MODELS_CMAKE_FILES_INSTALL_DIR  "/usr/lib/cmake/mqttsoa/")

include(GNUInstallDirs)

foreach ( file ${TARGET_INC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION "${CMAKE_INSTALL_PREFIX}/${dir}/mqttsoa")
endforeach()

install(FILES include/nlohman/json.hpp DESTINATION "${CMAKE_INSTALL_PREFIX}/${dir}/mqttsoa/nlohman")

install(TARGETS ${TARGET_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${MODELS_CMAKE_FILES}
    DESTINATION ${MODELS_CMAKE_FILES_INSTALL_DIR})