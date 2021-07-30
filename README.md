Port of the WIZnet W5100 to the ESP-IDF

This is a test app that runs the basic esp-idf
mqtt/ssl and esp_http_client examples over the
W5100 Ethernet chip.

The code assumes my specific design, such as
GPIOs, but the most important one is polling
status instead of using interrupts.

Base IDF rev.: [59aa60d52](https://github.com/espressif/esp-idf/commit/59aa60d52a190336b52cb89f53b0f4435aa3b029)
