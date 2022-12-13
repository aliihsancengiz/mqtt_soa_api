set(SERVER_SRC server.cpp)
set(SERVER_TARGET server)

set(CLIENT_SRC client.cpp)
set(CLIENT_TARGET client)

add_executable(${SERVER_TARGET} ${SERVER_SRC})
add_executable(${CLIENT_TARGET} ${CLIENT_SRC})

find_package(mqttsoa)

target_include_directories(${SERVER_TARGET}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ""
)

target_include_directories(${CLIENT_TARGET}
    PUBLIC
        ${mqttsoa_INCLUDE_DIRS}
    PRIVATE
        ""
)

target_link_libraries(${SERVER_TARGET}
    PUBLIC
        ${mqttsoa_LIBRARIES}
    PRIVATE
        ""
)

target_link_libraries(${CLIENT_TARGET}
    PUBLIC
        ${mqttsoa_LIBRARIES}
    PRIVATE
        ""
)
