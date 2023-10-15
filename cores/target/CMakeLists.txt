add_library(core)

set(CORE_DIR ${PROJECT_SOURCE_DIR}/arduino)

set(CORE_INCLUDES
    ${CORE_DIR}
    ${CORE_DIR}/api
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/api
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
    ${CORE_DIR}/api/Common.cpp
    ${CORE_DIR}/api/IPAddress.cpp
    ${CORE_DIR}/api/PluggableUSB.cpp
    ${CORE_DIR}/api/Print.cpp
    ${CORE_DIR}/api/Stream.cpp
    ${CORE_DIR}/api/WString.cpp

    api/ctimer.c
    api/interrupts.c
    api/pulse.cpp
    api/random.cpp
    api/time.c
    api/tone.cpp
    api/uart.cpp
    api/wiring_analog_read.c
    api/wiring_analog_write.c
    api/wiring_digital.c
    api/wiring_shift.c

    user_task.cpp
)