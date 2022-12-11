get_filename_component(MQTTSOA_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/../include/" ABSOLUTE)

set(mqttsoa_INCLUDE_DIRS "${MQTTSOA_INCLUDES}")
set(mqttsoa_LIBRARIES mqttsoa)
set(mqttsoa_LIBRARY_DIRS "lib")
