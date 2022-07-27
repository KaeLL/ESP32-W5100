Port of the WIZnet W5100 to the ESP-IDF

This is a test app that runs the basic esp-idf
mqtt/ssl and esp_http_client examples over the
W5100 Ethernet chip.

The code assumes my specific design, such as
GPIOs, but the most important one is polling
status instead of using interrupts.

Last update based on: espressif/esp-idf@36f49f361c001b49c538364056bc5d2d04c6f321