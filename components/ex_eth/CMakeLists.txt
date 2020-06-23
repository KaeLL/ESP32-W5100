
idf_component_register(
    SRCS "w5100_esp32/W5100/w5100.c" "w5100_esp32/W5100/w5100_socket.c" "w5100_esp32/W5100/w5100_spi.c" "w5100_esp32/emac/eth_mac.c" "w5100_esp32/emac/eth_main.c" "w5100_esp32/emac/eth_phy.c"
    INCLUDE_DIRS "w5100_esp32/W5100/include" "w5100_esp32/emac/include"
)