add_library(button)

set(BUTTON_DIR ${CMAKE_CURRENT_LIST_DIR})

set(BUTTON_INCLUDES
    ${BUTTON_DIR}
    ${BUTTON_DIR}/src
    CACHE PATH "BUTTON includes"
)

target_include_directories(
    button PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${BUTTON_INCLUDES}
)

target_sources(
    button PRIVATE
    ${BUTTON_DIR}/src/device_button.c
    ${BUTTON_DIR}/src/button_task.c
)