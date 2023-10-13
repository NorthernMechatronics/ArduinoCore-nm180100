add_library(variant)

set(VARIANT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(VARIANT_INCLUDES
    ${VARIANT_DIR}
    CACHE PATH "VARIANT includes"
)

target_include_directories(
    variant PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${SPI_INCLUDES}
    ${WIRE_INCLUDES}
)

target_sources(
    variant PRIVATE
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
    startup_gcc.c
)