idf_component_register(
    SRCS "mqtt_example.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES nvs_flash mqtt app_update
)