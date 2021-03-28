# Embed the server root certificate into the final binary
#
# (If this was a component, we would set COMPONENT_EMBED_TXTFILES here.)
idf_component_register(
    SRCS "esp_http_client_example.c"
    INCLUDE_DIRS "include"
    EMBED_TXTFILES howsmyssl_com_root_cert.pem
    PRIV_REQUIRES esp-tls esp_http_client
)