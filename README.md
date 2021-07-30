Port of the WIZnet W5100 to the ESP-IDF

This is a test app that runs the basic esp-idf
mqtt/ssl and esp_http_client examples over the
W5100 Ethernet chip.

The code assumes my specific design, such as
GPIOs, but the most important one is polling
status instead of using interrupts.

Base IDF rev.: [58022f859](https://github.com/espressif/esp-idf/commit/58022f8599401740979ac7657a57348285a70374)
