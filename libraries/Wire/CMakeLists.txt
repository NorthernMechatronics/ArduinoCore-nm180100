add_library(wire)

set(WIRE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(WIRE_INCLUDES
    ${WIRE_DIR}
    ${WIRE_DIR}/src
    CACHE PATH "WIRE includes"
)

target_include_directories(
    wire PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${WIRE_INCLUDES}
)

target_sources(
    wire PRIVATE
    ${WIRE_DIR}/src/Wire.cpp
)