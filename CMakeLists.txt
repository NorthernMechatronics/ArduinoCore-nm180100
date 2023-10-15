cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.13.0)

get_filename_component(APPLICATION ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

option(NM180100EVB "" OFF)
option(NM180410 "" OFF)

set(TARGET "nm180100" CACHE STRING "" FORCE)
if (NM180100EVB)
    set(BOARD "nm180100evb" CACHE STRING "" FORCE)
elseif (NM180410)
    set(BOARD "nm180410" CACHE STRING "" FORCE)
else()
    return()
endif()

option(RAT_LORAWAN_ENABLE "" ON)

project(${APPLICATION})

add_subdirectory(cores)
add_subdirectory(libraries/SPI)
add_subdirectory(libraries/Wire)
add_subdirectory(libraries/Servo)
add_subdirectory(libraries/LoRaWAN)
add_subdirectory(variants/${BOARD})

add_executable(${APPLICATION})
add_library(arduino STATIC
    $<TARGET_OBJECTS:hal>
    $<TARGET_OBJECTS:rtos>
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:spi>
    $<TARGET_OBJECTS:wire>
    $<TARGET_OBJECTS:servo>
    $<TARGET_OBJECTS:lorawan>
    $<TARGET_OBJECTS:lorawan_l4>
)

target_link_libraries(
    ${APPLICATION}
    PRIVATE
        variant
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
    ${SERVO_INCLUDES}
    ${LORAWAN_INCLUDES}
    ${LORAWAN_L4_INCLUDES}
)

target_sources(
    ${APPLICATION}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/test/test_sketch.cpp
    ${PROJECT_SOURCE_DIR}/cores/arduino/stub.c
)

add_custom_command(
    TARGET ${APPLICATION}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE_NAME:${APPLICATION}> $<TARGET_FILE_NAME:${APPLICATION}>.bin
)

add_dependencies(arduino hal rtos core spi wire servo lorawan lorawan_l4)
add_dependencies(${APPLICATION} arduino)

install(TARGETS arduino DESTINATION ${PROJECT_SOURCE_DIR}/variants/${BOARD}/lib)