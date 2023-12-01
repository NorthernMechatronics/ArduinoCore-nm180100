add_library(nm180100evb)

set(NM180100EVB_DIR ${CMAKE_CURRENT_LIST_DIR})

set(NM180100EVB_INCLUDES
    ${NM180100EVB_DIR}
    CACHE PATH "VARIANT includes"
)

target_include_directories(
    nm180100evb PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${SPI_INCLUDES}
    ${WIRE_INCLUDES}
)

target_sources(
    nm180100evb PRIVATE
    main.c
    serial.cpp
    serial1.cpp
    spi.cpp
    spi1.cpp
    spi2.cpp
    i2c.cpp
    i2c1.cpp
    i2c2.cpp
    i2c5.cpp
    variant.cpp
)