add_library(core)

set(CORE_DIR ${PROJECT_SOURCE_DIR}/arduino/api)

set(CORE_INCLUDES
    ${CORE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE PATH "CORE includes"
)

target_include_directories(
    core PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
)

target_sources(
    core PRIVATE
    ${CORE_DIR}/Common.cpp
    ${CORE_DIR}/IPAddress.cpp
    ${CORE_DIR}/PluggableUSB.cpp
    ${CORE_DIR}/Print.cpp
    ${CORE_DIR}/Stream.cpp
    ${CORE_DIR}/String.cpp
    api/wiring_digital.c
    user_task.c
)