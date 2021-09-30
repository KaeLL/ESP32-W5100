Port of the WIZnet W5100 to the ESP-IDF

This is a test app that runs the basic esp-idf
mqtt/ssl and esp_http_client examples over the
W5100 Ethernet chip.

The code assumes my specific design, such as
GPIOs, but the most important one is polling
status instead of using interrupts.

Base IDF rev.: [3e370c429](https://github.com/espressif/esp-idf/commit/3e370c4296247b349aa3b9a0076c05b9946d47dc)