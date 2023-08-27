add_library(servo)

set(SERVO_DIR ${CMAKE_CURRENT_LIST_DIR})

set(SERVO_INCLUDES
    ${SERVO_DIR}
    ${SERVO_DIR}/src
    CACHE PATH "Servo includes"
)

target_include_directories(
    servo PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${SERVO_INCLUDES}
)

target_sources(
    servo PRIVATE
    ${SERVO_DIR}/src/Servo.cpp
)