
idf_component_register(
    SRCS "mqtt_example.c"
    INCLUDE_DIRS "include"
    EMBED_TXTFILES mqtt_eclipseprojects_io.pem
    PRIV_REQUIRES mqtt app_update
)
