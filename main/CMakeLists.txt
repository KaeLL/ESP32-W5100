idf_component_register(
    SRCS "main.c"
    PRIV_REQUIRES driver w5100 ex_esp_http_client ex_mqtt
)