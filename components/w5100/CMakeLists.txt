
idf_component_register(
    SRC_DIRS port/src w5100_esp32/src .
    INCLUDE_DIRS include
    PRIV_INCLUDE_DIRS port/include w5100_esp32/include w5100_esp32/priv_includes
    PRIV_REQUIRES driver esp_eth esp_netif
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wmissing-prototypes)