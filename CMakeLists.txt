cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.13.0)

get_filename_component(APPLICATION ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

set(TARGET "nm180100" CACHE STRING "" FORCE)
set(BOARD "nm180100evb" CACHE STRING "" FORCE)

project(${APPLICATION})

add_subdirectory(cores)
add_subdirectory(libraries/SPI)
add_subdirectory(libraries/Wire)
add_subdirectory(variants/nm180100evb)

add_executable(${APPLICATION})
add_library(arduino STATIC
    $<TARGET_OBJECTS:hal>
    $<TARGET_OBJECTS:rtos>
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:spi>
    $<TARGET_OBJECTS:wire>
    $<TARGET_OBJECTS:variant>)

target_link_libraries(
    ${APPLICATION}
    PRIVATE
        arduino
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
    ${APPLICATION}
    PRIVATE
)

target_include_directories(
    ${APPLICATION}
    PRIVATE
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${CORE_INCLUDES}
    ${PROJECT_SOURCE_DIR}/variants/${BOARD}

    ${SPI_INCLUDES}
    ${WIRE_INCLUDES}
)

target_sources(
    ${APPLICATION}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/test/test_sketch.cpp
    ${PROJECT_SOURCE_DIR}/variants/stub.c
)

add_custom_command(
    TARGET ${APPLICATION}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE_NAME:${APPLICATION}> $<TARGET_FILE_NAME:${APPLICATION}>.bin
)

add_dependencies(arduino hal rtos core)
add_dependencies(${APPLICATION} arduino spi)

install(TARGETS arduino DESTINATION ${PROJECT_SOURCE_DIR}/variants/${BOARD}/lib)