add_library(spi)

set(SPI_DIR ${CMAKE_CURRENT_LIST_DIR})

set(SPI_INCLUDES
    ${SPI_DIR}/src
    CACHE PATH "SPI includes"
)

target_include_directories(
    spi PRIVATE
    ${CORE_INCLUDES}
    ${HAL_INCLUDES}
    ${SPI_INCLUDES}
)

target_sources(
    spi PRIVATE
    ${SPI_DIR}/src/SPI.cpp
)