add_library(lorawan_l4)

set(LORAWAN_L4_DIR ${CMAKE_CURRENT_LIST_DIR})

set(LORAWAN_L4_INCLUDES
    ${LORAWAN_L4_DIR}
    ${LORAWAN_L4_DIR}/src
    ${LORAWAN_L4_DIR}/src/soft-se
    CACHE PATH "LORAWAN_L4 includes"
)

target_include_directories(
    lorawan_l4 PRIVATE
    ${HAL_INCLUDES}
    ${RTOS_INCLUDES}
    ${LORAWAN_INCLUDES}
    ${LORAWAN_L4_INCLUDES}
)

target_compile_definitions(
    lorawan_l4 PRIVATE
    -DRAT_LORAWAN_ENABLE
    -DSOFT_SE
    -DCONTEXT_MANAGEMENT_ENABLED
)

target_sources(
    lorawan_l4 PRIVATE
    ${LORAWAN_L4_DIR}/src/soft-se/aes.c
    ${LORAWAN_L4_DIR}/src/soft-se/cmac.c
    ${LORAWAN_L4_DIR}/src/soft-se/soft-se.c
    ${LORAWAN_L4_DIR}/src/lmh_callbacks.c
    ${LORAWAN_L4_DIR}/src/lmhp_fragmentation.c
    ${LORAWAN_L4_DIR}/src/lorawan_se.c
    ${LORAWAN_L4_DIR}/src/lorawan_task.c
)