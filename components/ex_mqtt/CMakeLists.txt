
idf_component_register(
    INCLUDE_DIRS include
    SRCS mqtt_example.c
    EMBED_TXTFILES mqtt_eclipseprojects_io.pem
    PRIV_REQUIRES mqtt app_update
)
