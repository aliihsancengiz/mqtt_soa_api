set(TARGET_NAME "run_tests")

set(TEST_SRC
    templateTests.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${TARGET_NAME} ${TEST_SRC})
include_directories(${PROJECT_SOURCE_DIR}/inc)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ""
    PRIVATE
        Threads::Threads
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
)