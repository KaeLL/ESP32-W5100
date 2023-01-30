
idf_component_register(
    SRCS esp_http_client_example.c $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/protocol_examples_utils.c
    INCLUDE_DIRS include $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/include
    EMBED_TXTFILES howsmyssl_com_root_cert.pem postman_root_cert.pem
    PRIV_REQUIRES esp-tls esp_http_client
)

idf_component_optional_requires(PRIVATE esp_netif)