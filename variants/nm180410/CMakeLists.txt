add_library(nm180410)

set(NM180410_DIR ${CMAKE_CURRENT_LIST_DIR})

set(NM180410_INCLUDES
    ${NM180410_DIR}
    CACHE PATH "VARIANT includes"
)

target_include_directories(
    nm180410 PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${SPI_INCLUDES}
    ${WIRE_INCLUDES}
)

target_sources(
    nm180410 PRIVATE
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