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
    user_task.c
)