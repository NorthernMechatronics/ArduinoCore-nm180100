cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.13.0)

set(TARGET "nm180100" CACHE STRING "" FORCE)
set(BOARD "nm180100evb" CACHE STRING "" FORCE)

add_subdirectory(cores/nmsdk2)

project(${BOARD})

add_executable(${BOARD})

target_link_libraries(
    ${BOARD}
    PRIVATE
        rtos
        hal
    PUBLIC
        -T${PROJECT_SOURCE_DIR}/variants/${BOARD}/AMA3B1KK.ld
        -Wl,--start-group
        -lm
        -lc
        -lgcc
        -Wl,--end-group
        -Wl,--gc-sections
)

target_compile_definitions(
    ${BOARD}
    PRIVATE
)

target_include_directories(
    ${BOARD}
    PRIVATE
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${PROJECT_SOURCE_DIR}/variants/${BOARD}
)

target_sources(
    ${BOARD}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/variants/${BOARD}/startup_gcc.c
    ${PROJECT_SOURCE_DIR}/variants/${BOARD}/main.c
)

add_custom_command(
    TARGET ${BOARD}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE_NAME:${BOARD}> $<TARGET_FILE_NAME:${BOARD}>.bin
)

add_dependencies(${BOARD} hal rtos)