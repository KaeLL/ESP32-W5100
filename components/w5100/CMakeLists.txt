
idf_component_register(
    SRC_DIRS port/src w5100_esp32/src .
    INCLUDE_DIRS include
    PRIV_INCLUDE_DIRS port/include w5100_esp32/include w5100_esp32/priv_includes
    PRIV_REQUIRES driver ex_esp_http_client ex_mqtt
)