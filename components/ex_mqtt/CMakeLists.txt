idf_component_register(SRCS "mqtt_example.c"
                    INCLUDE_DIRS "include"
                    EMBED_TXTFILES mqtt_eclipse_org.pem
                    PRIV_REQUIRES mqtt)